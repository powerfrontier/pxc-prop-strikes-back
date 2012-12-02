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
	printf("ConnectionManager(): Primera instruccion\n");
 	fflush(stdout);
    SSL_load_error_strings();
	printf("ConnectionManager(): 2 instruccion\n");
 	fflush(stdout);
    SSL_library_init();
	printf("ConnectionManager(): 3 instruccion\n");
 	fflush(stdout);
    ERR_load_BIO_strings();
	printf("ConnectionManager(): 4 instruccion\n");
 	fflush(stdout);
    ERR_load_crypto_strings();
	printf("ConnectionManager(): 5 instruccion\n");
 	fflush(stdout);
    OpenSSL_add_all_algorithms();
	printf("ConnectionManager(): 6 instruccion\n");
 	fflush(stdout);
    t = NULL;
	printf("ConnectionManager(): 7 instruccion\n");
 	fflush(stdout);
    cCallB = NULL;
	printf("ConnectionManager(): Ok\n");
 	fflush(stdout);
}

ConnectionManager::~ConnectionManager() throw(){
	//TODO: KILL THREADS
	t = NULL;
	cCallB = NULL;
}

void ConnectionManager::listen(const std::string& port) throw(ConnectionException){
	printf("Listen(): 1 instruccion\n");
 	fflush(stdout);
	t = new std::thread(&ConnectionManager::listenThread, this, port);
	printf("Listen(): Ok\n");
 	fflush(stdout);
}


void ConnectionManager::setCallbackFunction(ConnectionCallback* callB) throw(){
	cCallB = callB;
}

void ConnectionManager::receive(Connection *con) throw (ConnectionException){
	con->receive();
}

void ConnectionManager::listenThread(const std::string& port) throw(ConnectionException){
	printf("ListenThread(): 1 instruccion\n");
 	fflush(stdout);
	BIO* bioStack, *cbio;
	printf("ListenThread(): 2 instruccion\n");
 	fflush(stdout);
	std::string str = port;
	printf("ListenThread(): 3 instruccion\n");
 	fflush(stdout);
	char buff[64];
	printf("ListenThread(): 4 instruccion\n");
 	fflush(stdout);
	str.copy(buff, str.size(), 0);
	printf("ListenThread(): 5 instruccion\n");
 	fflush(stdout);
	bioStack = BIO_new_accept(buff);
	printf("ListenThread(): 6 instruccion\n");
 	fflush(stdout);
	if (bioStack == NULL) {
		char message[] = "bioStack = NULL.\n";
 		print_ssl_error2(message, stdout);
	}
      printf("2");
    fflush(stdout);
	/* First call to BIO_accept() sets up accept BIO */
	if(BIO_do_accept(bioStack) <= 0) {
		char message[] = "bioStack <= 0.\n";
 		print_ssl_error2(message, stdout);
	}
    printf("3");
    fflush(stdout);
	while (1){

    printf("5");
    fflush(stdout);
		cbio = BIO_pop(bioStack);
    printf("YIPYIYPIY");
    fflush(stdout);
		if (cbio != NULL){
    printf("sHURMANO");
    fflush(stdout);
			Connection *c = new TCPConnection(cbio);
			c->setCallbackFunction(cCallB);
			std::thread t(&ConnectionManager::receive, this ,c);
		}
    printf("sHURPRIMO");
    fflush(stdout);
	/* Wait for incoming connection */
	if(BIO_do_accept(bioStack) <= 0) {
        		char message[] = "bioStack <= 0 x2.\n";
	 		print_ssl_error2(message, stdout);
    printf("OMG");
    fflush(stdout);
		}
		
	}
}

