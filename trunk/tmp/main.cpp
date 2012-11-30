#include <TestClasses.h>
#include <iostream>


int main (int argc, char** argv) {
	TestTransferableSent* sent = NULL;
	TestTransferableRcvd* rcvd = NULL;

	Transferable* tp = NULL;
	int id = 0;
	TransferableCreator* c = NULL;

	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new TestProfile());
	TransferableFactory::instance().setProtocol("0.1a");

	//Mi instrucción que quiero enviar
	sent = new TestTransferableSent();
	sent->m1(5);


	//Esto emula el funcionamiento de conexión
	tp = sent;		
	id = tp->type();	
	//Obtenemos el creador
	c = TransferableFactory::instance().creator(id);
	
	//Obtenemos una instrucción a ejecutar en el destino
	rcvd = static_cast<TestTransferableRcvd*>(c->create(tp->transferableObject()));
	delete tp; //Eliminamos la original

	rcvd->exec(NULL); //Exec de la instrucción. NULL debería ser la conexión que la recibe

	delete rcvd;
}

