#include <string>
#include <Datagram.h>
#include <Connection.h>
#include <TestClasses.h>
#include <iostream>
#include <vector>
#include <sstream>
int main(int argc, char** argv){
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new TestProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	std::vector<Connection*> conexiones;
	std::string ip(argv[1]);
	std::string port(argv[2]);
	printf("MainClient(): 1\n");
        fflush(stdout);
	int i = 0;
	Connection* n;
	for( i = 0; i < 100 ; ++i){
		n = new TCPConnectionSecurity();
		n->connect(ip, port);
		conexiones.push_back(n);
	}
//	Connection* n = new TCPConnectionSecurity();
//	while(!n->connect(ip, port)) { 
//		std::cout << "No se ha podido conectar. Se volvera a intentar en unos momentos.. "<< std::endl; 
//		sleep(3);
//	}
	printf("MainClient(): objeto Conexion creado\n");
        fflush(stdout);
	printf("MainClient(): Connected\n");
        fflush(stdout);
for(i = 0; i < 100; ++i){
//	if (n->isLinkOnline()){			
	std::ostringstream ostr;
	ostr << i;   
	std::string numberConnection = ostr.str();
	std::cout << "Conexion num: " << i << std::endl;	
	n = conexiones.at(i);
	TestTransferableSent100* sent100 = NULL;
	sent100 = new TestTransferableSent100((char*)numberConnection.c_str(),argv[4]);

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

	

//		}else{
//			std::cout << "Conexion perdida, Reconectando... "<< std::endl;
//			if(n->connect(ip, port)) {
//				std::cout << "Conexion Restablecida "<< std::endl; 
//			}else{
//				std::cout << "No se ha podido conectar. Se volvera a intentar en unos momentos.. "<< std::endl; 
//				sleep(3);
//			}
//		}
      }
while(1);
}


