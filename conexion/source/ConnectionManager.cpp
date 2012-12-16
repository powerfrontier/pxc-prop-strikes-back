#include "ConnectionManager.h"
#include <iostream>
#include <string>
#include <openssl/bio.h> // BIO objects for I/O
#include <openssl/ssl.h> // SSL and SSL_CTX for SSL connections
#include <openssl/err.h> // Error reporting
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>


#include <openssl/ssl.h>

#define KEYFILE "server.pem"
//#define PASSWORD "password"
#define CA_LIST "root.pem"
#define DHFILE "dh1024.pem"

/**
 * Simple log function
 */
void slog2(char* message) {
    fprintf(stdout, "%s", message);
}

/**
 * Print SSL error details
 */
void print_ssl_error2(char* message, FILE* out) {
 
    fprintf(out, "%s", message);
    fprintf(out, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
    fprintf(out, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    ERR_print_errors_fp(out);
}

/**
 * Print SSL error details with inserted content
 */
void print_ssl_error_23(char* message, char* content, FILE* out) {
 
    fprintf(out, message, content);
    fprintf(out, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
    fprintf(out, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    ERR_print_errors_fp(out);
}


ConnectionManager::ConnectionManager() throw(){
    /* call the standard SSL init functions */
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    cCallB = NULL;
    myClose = NULL;
}

ConnectionManager::~ConnectionManager() throw(){
	//TODO: KILL THREADS
	cCallB = NULL;
	myClose = NULL;
	if (tListen != NULL){
		delete tListen;
		tListen = NULL;
	}
}

void ConnectionManager::listen(const std::string& port) throw(ConnectionException){
	std::thread *t = new std::thread(&ConnectionManager::listenThread, this, port);
	tListenN.push_back(t);
}

void ConnectionManager::listenSecure(const std::string& port, bool bidirectional) throw(ConnectionException){
	std::thread *t = new std::thread(&ConnectionManager::listenThreadSecure, this, port, bidirectional);
	tListenN.push_back(t);
}

void ConnectionManager::setCallbackFunction(ConnectionCallback* callB) throw(){
	cCallB = callB;
}

void ConnectionManager::listenThread(const std::string& port) throw(ConnectionException){
	BIO* bioStack, *cbio;
	std::string str = port;
	char buff[64];
	str.copy(buff, str.size(), 0);
	bioStack = BIO_new_accept(buff);
	if (bioStack == NULL) {
		char message[] = "ListenThread(): bioStack = NULL.\n";
 		print_ssl_error2(message, stdout);
	}
	/* First call to BIO_accept() sets up accept BIO */
	if(BIO_do_accept(bioStack) <= 0) {
		char message[] = "ListenThread(): bioStack <= 0.\n";
 		print_ssl_error2(message, stdout);
	}
	while (1){
		cbio = BIO_pop(bioStack);
		if (cbio != NULL){
			Connection *c = new TCPConnection(cbio, port);
			if (cCallB != NULL){
				c->setCallbackFunction(cCallB);
			}
			c->receive();
		}
		/* Wait for incoming connection */
		printf("ESPERANDO CONEXION\n");
		if(BIO_do_accept(bioStack) <= 0) {
        		char message[] = "bioStack <= 0 x2.\n";
	 		print_ssl_error2(message, stdout);
		}
		
	}
}

void ConnectionManager::listenThreadSecure(const std::string& port, bool secureBidirectional) throw(ConnectionException){
	int sock,s;
	BIO *sbio;
	SSL_CTX *ctx;
	SSL *ssl;
	int r;
	pid_t pid;
   	char *pass;
	/* Build our SSL context*/
	auto meth =SSLv23_method();

	/* Set up a SIGPIPE handler */
	//signal(SIGPIPE,sigpipe_handle);
    
	ctx=SSL_CTX_new(meth);

	/* Load our keys and certificates*/
	if(!(SSL_CTX_use_certificate_chain_file(ctx,KEYFILE))){
		std::cerr << "Can't read certificate file" << KEYFILE << std::endl;
		return;
	}

//	pass = (char *)malloc(8);
//	strcpy(pass,PASSWORD);
//	SSL_CTX_set_default_passwd_cb(ctx,password_cb);
	if(!(SSL_CTX_use_PrivateKey_file(ctx,KEYFILE,SSL_FILETYPE_PEM))){
		std::cerr << "Can't read key file" << std::endl;
		return;
	}

	/* Load the CAs we trust*/
	if(!(SSL_CTX_load_verify_locations(ctx,CA_LIST,0))){
		std::cerr << "Can't read CA list" << std::endl;
		return;
	}


	DH *ret=0;
	BIO *bio;

	if ((bio=BIO_new_file(DHFILE,"r")) == NULL){
		std::cerr << "Couldn't open DH file" << std::endl;
		return;
	}

	ret=PEM_read_bio_DHparams(bio,NULL,NULL,NULL);
	BIO_free(bio);
	if(SSL_CTX_set_tmp_dh(ctx,ret)<0){
		std::cerr << "Couldn't set DH parameters" << std::endl;
		return;
	}

  
	//LISTEN
	struct sockaddr_in sin;
	int val=1;
    
	if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
		std::cerr << "Couldn't make socket" << std::endl;
		return;
	}
    
	memset(&sin,0,sizeof(sin));
	sin.sin_addr.s_addr=INADDR_ANY;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(atoi(port.c_str()));
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
    
	if(bind(sock,(struct sockaddr *)&sin,sizeof(sin))<0){
		std::cerr << "Couldn't bind" << std::endl;
		return;
	}
	
    	::listen(sock,5);  


	while(1){
		std::cout << "LISTENING " << std::endl;
		if((s=accept(sock,0,0))<0){
			std::cerr << "Problem accepting" << std::endl;
			continue;
		}

		ssl=SSL_new(ctx);
		sbio=BIO_new_socket(s,BIO_NOCLOSE);
		SSL_set_bio(ssl,sbio,sbio);
		if((r=SSL_accept(ssl)<=0)){
			std::cerr << "SSL accept error because..." << SSL_get_error(ssl, r) << std::endl;	
			continue;
		}else{
			TCPConnectionSecurity *c = new TCPConnectionSecurity(ssl, port);
			bool secureOk;
			if (secureBidirectional){
				//std::cout << "WHAT" << std::endl;
				//secureOk = c->checkCertificate();//TODO: IpS?
				//std::cout << "NOW" << std::endl;
			}else{
				secureOk = true;
			}
			if (secureOk){
				if (cCallB != NULL){
					c->setCallbackFunction(cCallB);
				}
				if (myClose != NULL){
					c->setCloseFunction(myClose);
				}
				c->receive();
			}else{
				c->close();
			}
		}
		
  	}
}

void ConnectionManager::setMyClose(ConnectionClosedListener *func){
	myClose = func;
}
