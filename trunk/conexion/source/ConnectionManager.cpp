#include "ConnectionManager.h"
#include <thread>


ConnectionManager::ConnectionManager() throw(){}

ConnectionManager::~ConnectionManager() throw(){
	//TODO: KILL THREADS
	cCallB = NULL;
}

void ConnectionManager::listen(const std::string& port) throw(ConnectionException){
	std::thread t(std::bind(listenThread(port));
}


void ConnectionManager::setCallbackFunction(ConnectionCallback* callB) throw(){
	cCallB = callB;
}

void ConnectionManager::receive(Connection con){
	con.receive();
}

void ConnectionManager::listenThread(const std::string& port) throw(ConnectionException){
	BIO* bioStack, *cbio;
	bioStack = BIO_new_accept("*:" + port_cstr());
	if (bioStack == NULL) {
		//TODO: ERROR
	}
  
	/* First call to BIO_accept() sets up accept BIO */
	if(BIO_do_accept(bioStack) <= 0) {
		//TODO:ERROR		
	}

	/* Wait for incoming connection */
	if(BIO_do_accept(bioStack) <= 0) {
        	//TODO:ERROR
	}
	while (1){
		cbio = BIO_pop(abio);
		if (cbio != NULL){
			Connection c = new TCPConnection(cbio);
			c.setCallbackFunction(cCallB);
			std::thread t(std::bind(receive,c));
		}
		
	}
}

