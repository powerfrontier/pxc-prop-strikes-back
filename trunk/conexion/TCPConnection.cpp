/* Standard headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
/* OpenSSL headers */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
 

#include "Connection.h"


BIO *sbio;

/**
 * Simple log function
 */
void slog(char* message) {
    fprintf(stdout, "%s", message);
}

/**
 * Print SSL error details
 */
void print_ssl_error(char* message, FILE* out) {
 
    fprintf(out, "%s", message);
    fprintf(out, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
    fprintf(out, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    ERR_print_errors_fp(out);
}
 
/**
 * Print SSL error details with inserted content
 */
void print_ssl_error_2(char* message, char* content, FILE* out) {
 
    fprintf(out, message, content);
    fprintf(out, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
    fprintf(out, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    ERR_print_errors_fp(out);
}

TCPConnection::TCPConnection() throw(){
    /* call the standard SSL init functions */
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
 
    /* seed the random number system - only really nessecary for systems without '/dev/random' */
    /* RAND_add(?,?,?); need to work out a cryptographically significant way of generating the seed */
}

TCPConnection::~TCPConnection() throw(){
	if (isLinkOnline())
		close();
}

bool TCPConnection::connect(std::string& ipAddr) throw(ConnectionException){
	sbio = NULL;
 
	/* Create a new connection */
	char *ipAddrToChar = new char[ipAddr.size()+1] ;
	strcpy(ipAddrToChar, ipAddr.c_str());
	sbio = BIO_new_connect(ipAddrToChar);
    	if (sbio == NULL) {
		char message[] = "Unable to create a new unencrypted BIO object.\n";
 		print_ssl_error(message, stdout);
        	return false;
    	}

	/* Verify successful connection */
	if (BIO_do_connect(sbio) != 1) {
		char message[] = "Unable to connect unencrypted.\n";
		print_ssl_error(message, stdout);
        	close();
        	return false;
    	}

	return true;
}

void TCPConnection::close() throw(){
    int r = 0;
    r = BIO_free(sbio);
    if (r == 0) {
        /* TODO:Error unable to free BIO */
    } 
}

bool TCPConnection::isLinkOnline() throw(){
	//TODO:crear "Ping"
	return true;
}

void TCPConnection::send(Transferable& message) throw (ConnectionException){
    char* buffer = (char *)&message.transferableObject();
    ssize_t length = message.size();
    ssize_t r = -1;
 
    while (r < 0) {
 
        r = BIO_write(sbio, buffer, length);
        if (r <= 0) {
 
            if (!BIO_should_retry(sbio)) {
 		char message[] ="BIO_read should retry test failed.\n";
                print_ssl_error(message, stdout);
                continue;
            }
 
            /* TODO:It would be prudent to check the reason for the retry and handle
             * it appropriately here */
        }
 
    }
 }

Transferable* TCPConnection::receive() throw(ConnectionException){
    ssize_t length = 256; //TODO: Fijar tamaño
    char buffer[length]; 

    ssize_t r = -1;
 
    while (r < 0) {
 
        r = BIO_read(sbio, buffer, length);
        if (r == 0) {
	    char message[] ="Reached the end of the data stream.\n";
            print_ssl_error(message, stdout);
            continue;
 
        } else if (r < 0) {
 
            if (!BIO_should_retry(sbio)) {
                char message[] ="BIO_read should retry test failed.\n";
                print_ssl_error(message, stdout);
                continue;
            }
 
            /* It would be prudent to check the reason for the retry and handle
             * it appropriately here */
        }

	//TODO: Tenemos el buffer lleno, convertir en transferable
	//Transferable = buffer
 
    };
}
