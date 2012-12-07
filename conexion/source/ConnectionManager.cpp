#include "ConnectionManager.h"
#include <iostream>
#include <string>

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
}

ConnectionManager::~ConnectionManager() throw(){
	//TODO: KILL THREADS
	cCallB = NULL;
	if (tListen != NULL){
		delete tListen;
		tListen = NULL;
	}
}

void ConnectionManager::listen(const std::string& port) throw(ConnectionException){
	std::thread *t = new std::thread(&ConnectionManager::listenThread, this, port);
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

