#include <stdlib.h>
#include <stdio.h>
#include "Connection.h"
#include <unistd.h>
#include <netinet/in.h>
#include<iostream>	//cout
#include<string.h>	//strlen
#include<string>	//string
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include<netdb.h>	//hostent
#include<sysConnect.h>
#include <netinet/in.h>

/* OpenSSL headers */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <thread>
TCPConnectionSecurity::TCPConnectionSecurity() throw() : Connection() {
    /* call the standard SSL init functions */
    CRYPTO_malloc_init(); // Initialize malloc, free, etc for OpenSSL's use
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    tListen = NULL;


/*************************/
	/* Create the TCP socket */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		std::cout << "DIE CREATING A SOCKET" << std::endl;
	}
/*************************/

//    setLinkOnline(false);
    /* seed the random number system - only really nessecary for systems without '/dev/random' */
    /* RAND_add(?,?,?); need to work out a cryptographically significant way of generating the seed */
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
    tListen = NULL;
    mPort = port;
    setLinkOnline(true);*/
    /* seed the random number system - only really nessecary for systems without '/dev/random' */
    /* RAND_add(?,?,?); need to work out a cryptographically significant way of generating the seed */
}

TCPConnectionSecurity::~TCPConnectionSecurity() throw(){
	if (isLinkOnline())
		close(false);
}

bool TCPConnectionSecurity::connect(const std::string& ipAddr, const std::string& port) throw(ConnectionException){
	/* Construct the server sockaddr_in structure */
	memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
	echoserver.sin_family = AF_INET;                  /* Internet/IP */
	echoserver.sin_addr.s_addr = inet_addr(ipAddr.c_str());  /* IP address */
	echoserver.sin_port = htons(atoi(port.c_str()));       /* server port */
	/* Establish connection */
	if (sysConnect(sock,(struct sockaddr *) &echoserver,sizeof(echoserver)) < 0) {
              std::cout << "COULDN'T CONNECT TO SERVER " << std::endl;
	}
/*	if (sbio != NULL) close();*/
	/* Create a new connection */
/*	std::string ipandport(ipAddr + ":" + port);
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
*/
	/* Verify successful connection */
/*	
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
	return true;*/
}

void TCPConnectionSecurity::close() throw(){
	close(false);
}

void TCPConnectionSecurity::close(bool threadListen) throw(){
/*	setLinkOnline(false);
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
*/
}

bool TCPConnectionSecurity::isLinkOnline() throw(){
	std::lock_guard<std::mutex> lk(mOnlineMutex);
	return online;
}

std::string TCPConnectionSecurity::getPort(){
	return mPort;
}

void TCPConnectionSecurity::setLinkOnline(bool b){
	std::lock_guard<std::mutex> lk(mOnlineMutex);
	online = b;
}

void TCPConnectionSecurity::send(Transferable& message) throw (ConnectionException){
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


	/* Send the word to the server */
	int echolen = strlen(buffer);
	if (sysSend(sock, buffer, echolen, 0) != echolen) {
		std::cout << "Missmatch in number of sent bytes" << std::endl;
	}
/*	ssize_t r;
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
*/
}

void TCPConnectionSecurity::sendAnswer(Transferable& message) throw (ConnectionException){
/*	size_t lengthMessage = message.size();
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
*/
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

        /* Receive the word back from the server */
	int received =0;
	while (received < lengthCommunication) {
		int bytes = 0;
		if ((bytes = recv(sock, bufsizeCommunication, lengthCommunication, 0)) < 1) {
			std::cout << "Failed to receive bytes from server" << std::endl;
		}
		received += bytes;
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
		throw ConnectionException(e.what());
	}

	//receive the Transferable
	char bufferMessage[sizeMessage];
	received = 0;
	while (received < sizeMessage) {
		int bytes = 0;
		if ((bytes = recv(sock, bufferMessage, sizeMessage, 0)) < 1) {
			std::cout << "Failed to receive bytes from server" << std::endl;
		}
		received += bytes;
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
/*	size_t r;
	if (isLinkOnline()){
		r = SSL_read(ssl, bufsizeCommunication, lengthCommunication);
	}else{
		r = 0;
	}
	while (r != lengthCommunication){
		if (r == 0) {
			std::cout << "Reached the end of the data stream" << std::endl;
			close(true);
			return;
		} else if (r < 0) {
			//if (!BIO_should_retry(sbio)) {
			//	char message[] ="BIO_read should retry test failed.\n";
			//	print_ssl_error(message, stdout);
			//	close(true);
			//	return;
			//}
			//TODO: ERROR
			std::cout << "Receive error GU1" << std::endl;
			close(true);
			return;			
		}
		if (isLinkOnline()){
			r = SSL_read(ssl, bufsizeCommunication, lengthCommunication);
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
		throw ConnectionException(e.what());
	}

	//receive the Transferable
	char bufferMessage[sizeMessage];
	if (isLinkOnline()){
		r = SSL_read(ssl, bufferMessage, sizeMessage);
	}else{
		r = 0;
	}
	while (r != sizeMessage){
		if (r == 0) {
			std::cout << "Reached the end of the data stream" << std::endl;
			close(true);
			return;
		} else if (r < 0) {
			//if (!BIO_should_retry(sbio)) {
			//	char message[] ="BIO_read should retry test failed.\n";
			//	print_ssl_error(message, stdout);
			//	close(true);
			//	return;
			//}
			//TODO: CONTROLAR ERROR
			std::cout << "Receive error GU2" << std::endl;
			close(true);
			return;			
		}
		if (isLinkOnline()){
			r = SSL_read(ssl, bufferMessage, sizeMessage);
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
*/
}
