#include <stdlib.h>
#include <stdio.h>
#include "Connection.h"

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

TCPConnection::TCPConnection() throw() : Connection() {
    /* call the standard SSL init functions */
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
 
    /* seed the random number system - only really nessecary for systems without '/dev/random' */
    /* RAND_add(?,?,?); need to work out a cryptographically significant way of generating the seed */
}

TCPConnection::TCPConnection(BIO* b) throw() : Connection() {
    /* call the standard SSL init functions */
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    sbio = b;
    /* seed the random number system - only really nessecary for systems without '/dev/random' */
    /* RAND_add(?,?,?); need to work out a cryptographically significant way of generating the seed */
}

TCPConnection::~TCPConnection() throw(){
	if (isLinkOnline())
		close();
}

bool TCPConnection::connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException){
	sbio = NULL;
 
	/* Create a new connection */
	char *ipAddrToChar = new char[ipAddr.size()+1] ;
	strcpy(ipAddrToChar, ipAddr.c_str());
	strcat(ipAddrToChar, ":");
	strcat(ipAddrToChar, port.c_str());
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
	char buffer[8];
	size_t length = message.size();
	
	// Send the size of Transferable
	ssize_t r = -1;
	while (r < 0){
		r = BIO_write(sbio, &length, sizeof(size_t)); 
		if (r <=0){
			if (!BIO_should_retry(sbio)) {
				char message[] = "BIO_read should retry test failed.\n";
				print_ssl_error(message, stdout);
				continue;
			}
		}else if (r== sizeof(size_t)){
			break;
		}else{
			throw ConnectionException("TCPConnection Wrong: Guru meditation 1");
		}
	}

	// Send the protocol
	r = -1;	
	while (r< 0){
		std::string protocol = TransferableFactory::instance().protocol();
		strcpy(buffer, protocol.c_str());
		r = BIO_write(sbio, buffer, 8);
		if (r<=0) {
                        if (!BIO_should_retry(sbio)) {
                                char message[] ="BIO_read should retry test failed.\n";
                                print_ssl_error(message, stdout);
                                continue;
                        }
                }else if (r==sizeof(int)){
                        break;
                }else{
                        throw ConnectionException("TCPConnection Wrong: Guru meditation 2");
		}
	}
	int type; 
	try{
		type = message.type();
	}catch(TransferableVersionException& e){
		throw ConnectionException(e.what());
	}

	// Send the instruction
	r = -1;	
	while (r < 0){
		r = BIO_write(sbio, (char*) &type, sizeof(int));
		if (r<=0) {
			if (!BIO_should_retry(sbio)) {
                		char message[] ="BIO_read should retry test failed.\n";
	                        print_ssl_error(message, stdout);
        	                continue;
			}
		}else if (r==sizeof(int)){
			break;
		}else{
                        throw ConnectionException("TCPConnection Wrong: Guru meditation 3");
		}
	}
	
	// Send the Transferable
	r = -1;
	while (r < 0) {
		r = BIO_write(sbio,(char *)message.transferableObject(), length);
		if (r <= 0) {
			if (!BIO_should_retry(sbio)) {
				char message[] ="BIO_read should retry test failed.\n";
				print_ssl_error(message, stdout);
				continue;
			}
		}else if(r == length){
			break;
		}else{
                        throw ConnectionException("TCPConnection Wrong: Guru meditation 4");
		}
 	}
}


/*

*/
/*void TCPConnection::receive() throw(ConnectionException){
	TODO: Receive ha de crear un thread que ejecuta receiveThread
}

void TCPConnection::receiveThread(){
	while(1){
		this.receiveTransfThread();
	}
}*/
// TODO:Cambiar la cabecera cuando el thread creado
//void TCPConnection::receiveTransfThread() throw(ConnectionException){
void TCPConnection::receive() throw(ConnectionException){
	size_t length;
	char bufsize[sizeof(size_t)];
	char protocol[8];
	int instruction;

	//receive the size of transferable
	ssize_t r = -1;
	while (r<0){
		r = BIO_read(sbio, &length, sizeof(size_t));
	 	if (r == 0){
			char message[] = "The size of the packet is incorrect.\n";
			print_ssl_error(message, stdout);
			continue;
		}else if (r<0){
			if (!BIO_should_retry(sbio)){
				char message[] = "BIO_read should retry test failed.\n";
				print_ssl_error(message, stdout);
				continue;
			}
		}else if (r == sizeof(size_t)){
			break;
		}else{ 
                        throw ConnectionException("TCPConnection Wrong: Guru meditation 5");
		}
	}
	
	//receive protocol
	r = -1;
	while (r < 0){
		r = BIO_read(sbio, protocol, 8);
                if (r == 0){
                        char message[] = "The size of the packet is incorrect.\n";
                        print_ssl_error(message, stdout);
                        continue;
                }else if (r<0){
                        if (!BIO_should_retry(sbio)){
                                char message[] = "BIO_read should retry test failed.\n";
                                print_ssl_error(message, stdout);
                                continue;
                        }
                }else if (r == 8){
                        break;
                }else{ 
                        throw ConnectionException("TCPConnection Wrong: Guru meditation 6");
                }

	}

	//receive the instruction
	r = -1;
	while (r < 0){
                r = BIO_read(sbio, &instruction, sizeof(int));
                if (r == 0){
                        char message[] = "The size of the packet is incorrect.\n";
                        print_ssl_error(message, stdout);
                        continue;
                }else if (r<0){
                        if (!BIO_should_retry(sbio)){
                                char message[] = "BIO_read should retry test failed.\n";
                                print_ssl_error(message, stdout);
                                continue;
                        }
                }else if (r == sizeof(int)){
                        break;
                }else{
                        throw ConnectionException("TCPConnection Wrong: Guru meditation 7");
                }
	}
	TransferableCreator* c;
	try {
		TransferableFactory::instance().setProtocol(protocol);
		c = TransferableFactory::instance().creator(instruction);
	}catch(TransferableVersionException& e){
		throw ConnectionException(e.what());
	}

	//receive the Transferable
	char buffer[length];
	r = -1;
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
        	}else if (r == length){
			Transferable *t;
			try{
				t = c->create((void*)buffer);
				if (!mCallback) t->exec(this);
				else mCallback->callbackFunction(t, this);
			}catch(TransferableVersionException& e){
				throw ConnectionException(e.what());
			}
		}else{
			throw ConnectionException("TCPConnection Wrong: Guru meditation 8");
		}
	}
	
}
