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
    tListen = NULL;
    sbio =  NULL;
    setLinkOnline(false);
    /* seed the random number system - only really nessecary for systems without '/dev/random' */
    /* RAND_add(?,?,?); need to work out a cryptographically significant way of generating the seed */
}

TCPConnection::TCPConnection(BIO* b, std::string port) throw() : Connection() {
    /* call the standard SSL init functions */
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    sbio = b;
    tListen = NULL;
    mPort = port;
    setLinkOnline(true);
    /* seed the random number system - only really nessecary for systems without '/dev/random' */
    /* RAND_add(?,?,?); need to work out a cryptographically significant way of generating the seed */
}

TCPConnection::~TCPConnection() throw(){
	if (isLinkOnline())
		close(false);
}

bool TCPConnection::connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException){
	if (sbio != NULL) close();
	/* Create a new connection */
	std::string ipandport(ipAddr + ":" + port);
	mPort = port;
	std::cout << "IP: ";
	std::cout << ipandport << std::endl;
	
	char * writable = new char[ipandport.size() + 1];
	std::copy(ipandport.begin(), ipandport.end(), writable);
	writable[ipandport.size()] = '\0'; 
	sbio = BIO_new_connect(writable);
	delete[] writable;
    	if (sbio == NULL) {
		char message[] = "Unable to create a new unencrypted BIO object.\n";
 		print_ssl_error(message, stdout);
		close(false);
        	return false;
    	}

	/* Verify successful connection */
	
	bool repeat;
	repeat = true;
	while (repeat){
		if (BIO_do_connect(sbio) != 1) {
			if (BIO_should_retry(sbio)){
				repeat = true;
			}else{
				repeat = false;
	       		 	close(false);
				char message[] = "Unable to connect unencrypted.\n";
				print_ssl_error(message, stdout);
	      	 	 	return false;
			}

    		}else{
			repeat = false;
		}
	}
	setLinkOnline(true);
	receive();
	return true;
}

void TCPConnection::close() throw(){
	close(false);
}

void TCPConnection::close(bool threadListen) throw(){
	setLinkOnline(false);
	int r = 0;
	if (sbio != NULL){
		if (!threadListen && tListen != NULL){
			std::cout << "CERRANDO LISTEN" << std::endl;
			tListen->join();
			delete tListen;
		}
		tListen = NULL;
		std::cout << "CERRANDO SOCKET" << std::endl;
		r = BIO_free(sbio);
		if (r == 0) {
			std::cout << "ERROR AL CERRAR SOCKET" << std::endl;
		} 
		sbio = NULL;
		std::cout << "CONEXION CERRADA" << std::endl;
	}else{
		std::cout << "LA CONEXION ESTABA CERRADA" << std::endl;
	}
}

bool TCPConnection::isLinkOnline() throw(){
	std::lock_guard<std::mutex> lk(mOnlineMutex);
	return online;
}

std::string TCPConnection::getPort(){
	return mPort;
}

void TCPConnection::setLinkOnline(bool b){
	std::lock_guard<std::mutex> lk(mOnlineMutex);
	online = b;
}

void TCPConnection::send(Transferable& message) throw (ConnectionException){
	size_t lengthMessage = message.size();
	size_t lengthPacket = sizeof(size_t) + 8 + sizeof(int) + lengthMessage;
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
	ssize_t r;
	if (isLinkOnline()){
		r = BIO_write(sbio,buffer, lengthPacket);
	}else{
		r = 0;
	}
	while (r != lengthPacket){
		if (r == 0) {	
			close(false);
			return;
		}else if (r < 0){
			if (!BIO_should_retry(sbio)) {
				char message[] ="TCPConnection::send() BIO_read should retry test failed.\n";
				print_ssl_error(message, stdout);
				close(false);
				return;
			}
		}else{
			char message[] = "TCPConnection::send() Wrong: Guru meditation 1\n";
			print_ssl_error(message, stdout);
			close(false);
			return;
		}
		if (isLinkOnline()){
			r = BIO_write(sbio,buffer, lengthPacket);
		}else{
			r = 0;
		}
	}
	sleep(1); // Para que no pete cuando envias muchos sends seguidos (le da tiempo a receive a actuar)

}

void TCPConnection::sendAnswer(Transferable& message) throw (ConnectionException){
	size_t lengthMessage = message.size();
	size_t lengthPacket = sizeof(size_t) + 8 + sizeof(int) + lengthMessage;
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
	ssize_t r;
	if (isLinkOnline()){
		r = BIO_write(sbio,buffer, lengthPacket);
	}else{
		r = 0;
	}
	while (r != lengthPacket){
		if (r == 0) {	
			close(true);
			return;
		}else if (r < 0){
			if (!BIO_should_retry(sbio)) {
				char message[] ="TCPConnection::send() BIO_read should retry test failed.\n";
				print_ssl_error(message, stdout);
				close(true);
				return;
			}
		}else{
			char message[] = "TCPConnection::send() Wrong: Guru meditation 1\n";
			print_ssl_error(message, stdout);
			close(true);
			return;
		}
		if (isLinkOnline()){
			r = BIO_write(sbio,buffer, lengthPacket);
		}else{
			r = 0;
		}
	}
	sleep(1); // Para que no pete cuando envias muchos sends seguidos (le da tiempo a receive a actuar)
}


void TCPConnection::receive() throw(ConnectionException){
	if (tListen == NULL) tListen = new std::thread(&TCPConnection::receiveThread, this);
}

void TCPConnection::receiveThread(){
	while(isLinkOnline()){
		try{
			receiveTransfThread();
		}catch (std::exception e){	
			std::cout << e.what() << std::endl;
			fflush(stdout);
		}
	}
}

void TCPConnection::receiveTransfThread() throw(ConnectionException){
	size_t sizeMessage;
	size_t lengthCommunication = sizeof(size_t)+8+sizeof(int);
	char bufsizeCommunication[lengthCommunication];
	char protocol[8];
	int instruction;
	size_t r;
	if (isLinkOnline()){
		r = BIO_read(sbio, bufsizeCommunication, lengthCommunication);
	}else{
		r = 0;
	}
	while (r != lengthCommunication){
		if (r == 0) {
			std::cout << "Reached the end of the data stream" << std::endl;
			close(true);
			return;
		} else if (r < 0) {
			if (!BIO_should_retry(sbio)) {
				char message[] ="BIO_read should retry test failed.\n";
				print_ssl_error(message, stdout);
				close(true);
				return;
			}
		}
		if (isLinkOnline()){
			r = BIO_read(sbio, bufsizeCommunication, lengthCommunication);
		}else{
			r = 0;
		}
       	}	
	memcpy(&sizeMessage, bufsizeCommunication, sizeof(size_t));
	memcpy(protocol, bufsizeCommunication+sizeof(size_t), 8);
	memcpy(&instruction, bufsizeCommunication+sizeof(size_t) + 8, sizeof(int));

	TransferableCreator* c;
	try {
		TransferableFactory::instance().setProtocol(protocol);
		c = TransferableFactory::instance().creator(instruction);
	}catch(TransferableVersionException& e){
		std::cout << "Protocol or instruction received incorrect" << std::endl;
		std::cout << "Protocol received: " << protocol << " Instruction received: " << instruction << std::endl;
		throw ConnectionException(e.what());
	}

	//receive the Transferable
	char bufferMessage[sizeMessage];
	if (isLinkOnline()){
		r = BIO_read(sbio, bufferMessage, sizeMessage);
	}else{
		r = 0;
	}
	while (r != sizeMessage){
		if (r == 0) {
			std::cout << "Reached the end of the data stream" << std::endl;
			close(true);
			return;
		} else if (r < 0) {
			if (!BIO_should_retry(sbio)) {
				char message[] ="BIO_read should retry test failed.\n";
				print_ssl_error(message, stdout);
				close(true);
				return;
			}
		}
		if (isLinkOnline()){
			r = BIO_read(sbio, bufferMessage, sizeMessage);
		}else{
			r = 0;
		}
	}
       	
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
}
