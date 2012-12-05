#include <string>
#include "Datagram.h"
#include "Connection.h"
#include <TestClasses.h>
#include <iostream>

int main(int argc, char** argv){
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new TestProfile());
	TransferableFactory::instance().setProtocol("0.1a");

	printf("MainClient(): 1\n");
        fflush(stdout);
	Connection* n = new TCPConnection();
	if(n->connect(std::string(argv[1]),std::string("3456"))) { std::cout << "3456 conectado" << std::endl;}else{ std::cout<<"3456PAM!"<<std::endl;}
	Connection* n2 = new TCPConnection();
	if(n2->connect(std::string(argv[1]),std::string("3457"))) { std::cout << "3457 conectado" << std::endl;}else{ std::cout<<"3457PAM!"<<std::endl;}
	Connection* n3 = new TCPConnection();
	if(n3->connect(std::string(argv[1]),std::string("3458"))) { std::cout << "3458 conectado" << std::endl;}else{ std::cout<<"3458PAM!"<<std::endl;}
	Connection* n4 = new TCPConnection();
	if(n4->connect(std::string(argv[1]),std::string("3459"))) { std::cout << "3459 conectado" << std::endl;}else{ std::cout<<"3459PAM!"<<std::endl;}
	printf("MainClient(): objeto Conexion creado\n");
        fflush(stdout);
		printf("MainClient(): Connected\n");
        	fflush(stdout);

		TestTransferableSent* sent = NULL;
		sent = new TestTransferableSent();
		int i = 0;
		sent->m1(i);
		printf("MainClient(): Sending...\n");
		fflush(stdout);
		n->send(*sent);
		printf("MainClient(): sent\n");
       		fflush(stdout);
		while (1){
		}
}


