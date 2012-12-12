#include <stdlib.h>
#include <stdio.h>
#include "Connection.h"
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>	//cout
#include <string.h>	//strlen
#include <string>	//string
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <netdb.h>	//hostent
#include <netinet/in.h>
#include <signal.h>

/* OpenSSL headers */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <thread>

#define CA_LIST "root.pem"
//#define RANDOM  "random.pem"
#define KEYFILE "client.pem"
#define PASSWORD "password"



TCPConnectionSecurity::TCPConnectionSecurity() throw() : Connection() {
	/* call the standard SSL init functions */
	CRYPTO_malloc_init(); // Initialize malloc, free, etc for OpenSSL's use
	SSL_load_error_strings();
	SSL_library_init();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();

	tListen = NULL;
	/* Create the TCP socket */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		std::cout << "DIE CREATING A SOCKET" << std::endl;
	}
	
//***
	/* Build our SSL context*/

    	auto *meth = SSLv23_method();

	/* Set up a SIGPIPE handler */
	signal(SIGPIPE, SIG_IGN);

	/* Create our context*/
	ctx=SSL_CTX_new(meth);

	/* Load our keys and certificates*/
	if(!(SSL_CTX_use_certificate_chain_file(ctx, KEYFILE))){
		std::cout << "Can't read certificate file" << std::endl;
	}
	pass = (char *)malloc(8);
	strcpy(pass,PASSWORD);
//	SSL_CTX_set_default_passwd_cb(ctx,password_cb);
	if(!(SSL_CTX_use_PrivateKey_file(ctx, KEYFILE,SSL_FILETYPE_PEM))){
		std::cout << "Can't read key file" << std::endl;
	}

	/* Load the CAs we trust*/
	if(!(SSL_CTX_load_verify_locations(ctx, CA_LIST,0))){
		std::cout << "Can't read CA list" << std::endl;
	}

//***

	setLinkOnline(false);
}

TCPConnectionSecurity::TCPConnectionSecurity(SSL* c, std::string port) throw() : Connection() {
    /* call the standard SSL init functions */
   /* CRYPTO_malloc_init(); // Initialize malloc, free, etc for OpenSSL's use
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    ssl= c;
    tListen = NULL;*/
	ssl = c;
	mPort = port;
	setLinkOnline(true);
}

TCPConnectionSecurity::~TCPConnectionSecurity() throw(){
	if (isLinkOnline())
		close(false);
}

bool TCPConnectionSecurity::connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException){
	/* Construct the server sockaddr_in structure */
	struct sockaddr_in echoserver;
	memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
	echoserver.sin_family = AF_INET;                  /* Internet/IP */
	echoserver.sin_addr.s_addr = inet_addr(ipAddr.c_str());  /* IP address */
	echoserver.sin_port = htons(atoi(port.c_str()));       /* server port */
	/* Establish connection */
	if (::connect(sock,(struct sockaddr *) &echoserver,sizeof(echoserver)) < 0) {
		std::cout << "COULDN'T CONNECT TO SERVER " << std::endl;
		return false;
	}
	/* Connect the SSL socket */
	ssl=SSL_new(ctx);
	sbio=BIO_new_socket(sock,BIO_NOCLOSE);
	SSL_set_bio(ssl,sbio,sbio);

	if(SSL_connect(ssl)<=0){
		std::cout << "SSL connect error" << std::endl;
		return false;
	}

	X509 *peer;
	char peer_CN[256];
	if(SSL_get_verify_result(ssl)!=X509_V_OK){
		std::cerr << "Certificate doesn't verify " << std::endl ;
		std::cerr << "The expected certificate is "<< X509_V_OK << " The received certificate is " << SSL_get_verify_result(ssl)  << std::endl ;
		return false;
	}
	/*Check the common name*/
	peer=SSL_get_peer_certificate(ssl);
	X509_NAME_get_text_by_NID(X509_get_subject_name(peer),NID_commonName, peer_CN, 256);
	if(strcasecmp(peer_CN,ipAddr.c_str())){
		std::cout << "Common name doesn't match host name"<<std::endl;
		return false;
	}
	mPort = port;
	setLinkOnline(true);
	receive();
	return true;
}

void TCPConnectionSecurity::close() throw(){
	close(false);
}

void TCPConnectionSecurity::close(bool threadListen) throw(){
	
}

bool TCPConnectionSecurity::isLinkOnline() throw(){
	std::lock_guard<std::mutex> lk(mOnlineMutex);
	return online;
}

const std::string& TCPConnectionSecurity::getPort(){
	return mPort;
}

void TCPConnectionSecurity::setLinkOnline(bool b){
	std::lock_guard<std::mutex> lk(mOnlineMutex);
	online = b;
}

void TCPConnectionSecurity::send(Transferable& message) throw (ConnectionException){
	size_t lengthMessage = message.size();
	int lengthPacket = sizeof(size_t) + 8 + sizeof(int) + lengthMessage;
	char buffer[lengthPacket];
	std::string protocol = TransferableFactory::instance().protocol();
	if (protocol.size() > 8){	
		std::cout << "Error using Transferable, size of protocol incorrect, it must be equal or less than 8" << std::endl;
		return;
	}
	int type; 
	try{
		type = message.type();
	}catch(TransferableVersionException& e){
		std::cout << "Error using Transferable, message.type incorrect " << std::endl;
		std::cout << e.what() << std::endl;
		return;
	}
	memcpy(buffer, (char *) &lengthMessage, sizeof(size_t));
	memcpy(buffer+sizeof(size_t), protocol.c_str(), 8);
	memcpy(buffer+sizeof(size_t)+8, (char*) &type, sizeof(int));
	memcpy(buffer+sizeof(size_t)+8+sizeof(int), (char*) message.transferableObject(), lengthMessage);



	/* Send the word to the server */
	int r;
	r=SSL_write(ssl,buffer,lengthPacket);
	switch(SSL_get_error(ssl,r)){      
		case SSL_ERROR_NONE:
			if(lengthPacket!=r)
				std::cerr << "Incomplete write!" << std::endl;
			break;
		default:
			std::cerr << "SSL write problem" << std::endl;
	}
/*	if (::send(sock, buffer, lengthPacket, 0) != lengthPacket) {
		std::cout << "Missmatch in number of sent bytes" << std::endl;
	}*/
	sleep(1);
}

void TCPConnectionSecurity::sendAnswer(Transferable& message) throw (ConnectionException){
	send(message);
}


void TCPConnectionSecurity::receive() throw(ConnectionException){
	if (tListen == NULL) tListen = new std::thread(&TCPConnectionSecurity::receiveThread, this);
}

void TCPConnectionSecurity::receiveThread(){
	while(isLinkOnline()){
		try{
			receiveTransfThread();
		}catch (std::exception e){	
			std::cout << e.what() << std::endl;
			fflush(stdout);
		}
	}
}

void TCPConnectionSecurity::receiveTransfThread() throw(ConnectionException){
	size_t sizeMessage;
	size_t lengthCommunication = sizeof(size_t)+8+sizeof(int);
	char bufsizeCommunication[lengthCommunication];
	char protocol[8];
	int instruction;
	
	//BIO *io,*ssl_bio;
    
    	
	/* Receive the word back from the server */
	int r;
	r=SSL_read(ssl,bufsizeCommunication,lengthCommunication);
	switch(SSL_get_error(ssl,r)){
		case SSL_ERROR_NONE:
			memcpy(&sizeMessage, bufsizeCommunication, sizeof(size_t));
			memcpy(protocol, bufsizeCommunication+sizeof(size_t), 8);
			memcpy(&instruction, bufsizeCommunication+sizeof(size_t) + 8, sizeof(int));
			break;
		case SSL_ERROR_ZERO_RETURN:
			close(true);
			return;
		case SSL_ERROR_SYSCALL:
			close(true);
			std::cerr << "SSL Error: Premature close" << std::endl;
			return;
		default:
			std::cerr << "SSL read problem" << std::endl;
	}
	
	/*io=BIO_new(BIO_f_buffer());
    	ssl_bio=BIO_new(BIO_f_ssl());
    	BIO_set_ssl(ssl_bio,ssl,BIO_CLOSE);
    	BIO_push(io,ssl_bio);
	int received;
	received =BIO_gets(io,bufsizeCommunication,lengthCommunication);
	switch(SSL_get_error(ssl,received)){
		case SSL_ERROR_NONE:
			memcpy(&sizeMessage, bufsizeCommunication, sizeof(size_t));
			memcpy(protocol, bufsizeCommunication+sizeof(size_t), 8);
			memcpy(&instruction, bufsizeCommunication+sizeof(size_t) + 8, sizeof(int));
			break;
		default:
			std::cerr << "SSL read problem" << std::endl;
	}
	*/
        
	/*int received =0;
	while (received < lengthCommunication) {
		int bytes = 0;
		if ((bytes = recv(sock, bufsizeCommunication, lengthCommunication, 0)) < 1) {
			std::cout << "Failed to receive bytes from server" << std::endl;
		}
		received += bytes;
	}*/


	TransferableCreator* c;
	try {
		TransferableFactory::instance().setProtocol(protocol);
		c = TransferableFactory::instance().creator(instruction);
	}catch(TransferableVersionException& e){
		std::cout << "Protocol or instruction received incorrect" << std::endl;
		throw ConnectionException(e.what());
	}

	//receive the Transferable
	char bufferMessage[sizeMessage];
	
	r=SSL_read(ssl,bufferMessage,sizeMessage);
	switch(SSL_get_error(ssl,r)){
		case SSL_ERROR_NONE:
			Transferable *t;
			try{
				t = c->create((void*)bufferMessage);
				if (!mCallback) {
					t->exec(this);
					delete t;
				}else{
					mCallback->callbackFunction(t, this);
				}
			}catch(TransferableVersionException& e){
				throw ConnectionException(e.what());
			}   
			break;
		case SSL_ERROR_ZERO_RETURN:
			close(true);
			return;
		case SSL_ERROR_SYSCALL:
			close(true);
			std::cerr << "SSL Error: Premature close" << std::endl;
			return;
		default:
			std::cerr << "SSL read problem" << std::endl;
	}
	/*received =BIO_gets(io,bufferMessage,sizeMessage);
	switch(SSL_get_error(ssl,received)){
		case SSL_ERROR_NONE:
			Transferable *t;
			try{
				t = c->create((void*)bufferMessage);
				if (!mCallback) {
					t->exec(this);
					delete t;
				}else{
					mCallback->callbackFunction(t, this);
				}
			}catch(TransferableVersionException& e){
				throw ConnectionException(e.what());
			}   
			break;
		default:
			std::cerr << "SSL read problem" << std::endl;
	}
	*/
	/*received = 0;
	while (received < sizeMessage) {
		int bytes = 0;
		if ((bytes = recv(sock, bufferMessage, sizeMessage, 0)) < 1) {
			std::cout << "Failed to receive bytes from server" << std::endl;
		}
		received += bytes;
        }*/
    
}
