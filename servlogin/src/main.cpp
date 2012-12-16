#include <Login.h>
#include <LoginProfile.h>
#include <iostream>
#include <my_global.h>
#include <mysql.h>


using namespace std;




int main() {
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new LoginProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	Login::instance().initializeLogin();
	Login::instance().initializeManager();

	printf("Login: Estoy en listen\n");
printf("MySQL client version: %s\n", mysql_get_client_info());
 	fflush(stdout);

	while (1){
		//sleep(50);
	}
}
