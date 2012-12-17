#include <string>
#include <Datagram.h>
#include <Connection.h>
#include <TestClasses.h>
#include <iostream>

int main(int argc, char** argv){
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new TestProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	std::string ip(argv[1]);
	std::string port(argv[2]);
	printf("MainClient(): 1\n");
        fflush(stdout);
	Connection* n = new TCPConnectionSecurity();
	while(!n->connect(ip, port)) { 
		std::cout << "No se ha podido conectar. Se volvera a intentar en unos momentos.. "<< std::endl; 
		sleep(3);
	}
	printf("MainClient(): objeto Conexion creado\n");
        fflush(stdout);
	printf("MainClient(): Connected\n");
        fflush(stdout);

	if (n->isLinkOnline()){			
			TestTransferableSent100* sent100 = NULL;
			sent100 = new TestTransferableSent100(argv[3],argv[4]);
		
			printf("MainClient(): Sending100...\n");
			fflush(stdout);
			n->send(*sent100);
			printf("MainClient(): sent100\n");
	       		fflush(stdout);
			delete sent100;

			
			printf("Ahora hacemos logout\n");
			fflush(stdout);
			TestTransferableSent102* sent102 = NULL;
			sent102 = new TestTransferableSent102(0,0);
			printf("MainClient(): Sending102...\n");
			fflush(stdout);
			n->send(*sent102);
			printf("MainClient(): sent102\n");
	       		fflush(stdout);
			delete sent102;
		}else{
			std::cout << "Conexion perdida, Reconectando... "<< std::endl;
			if(n->connect(ip, port)) {
				std::cout << "Conexion Restablecida "<< std::endl; 
			}else{
				std::cout << "No se ha podido conectar. Se volvera a intentar en unos momentos.. "<< std::endl; 
				sleep(3);
			}
		}
      

}

