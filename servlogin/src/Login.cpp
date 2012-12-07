#include <Login.h>
#include <LoginProfile.h>

void Login::initializeManager(){
    manager = new ConnectionManager();
    manager->listen(CONTROL_PORT);
    manager->listen(CLIENT_PORT);
}

Login::~Login(){
  delete manager;
}

int main() {
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new LoginProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	Login::instance().initializeManager();
	printf("Login: Estoy en listen\n");
 	fflush(stdout);
	while (1){}
}
