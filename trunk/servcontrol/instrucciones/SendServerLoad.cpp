#include "SendServerLoad.h"

void SendServerLoad::exec(Connection* c) const throw(){
	//Inserir la carrega a la carrega del server que toca i actualitzar la mascara on toca també
	s->carga = carga;
	rebut_mutex->lock();
	*rebut = *rebut || (1 << s->id);
	rebut_mutex->unlock();

}



