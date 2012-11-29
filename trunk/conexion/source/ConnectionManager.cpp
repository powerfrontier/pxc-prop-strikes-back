#include "ConnectionManager.h"



ConnectionManager::ConnectionManager() throw();
ConnectionManager::~ConnectionManager() throw();
void ConnectionManager::listenPort(const std::string& port) throw(ConnectionException);
void ConnectionManager::setCallbackFunction(ConnectionCallback*) throw();

void ConnectionManager::receive

void ConnectionManager::listen(const std::string& port) throw(ConnectionException){
	bioStack = BIO_new_accept("*:" + port_cstr());
	if (bioStack == NULL) {
		//TODO: ERROR
	}
  
	BIO *abio, *cbio, *cbio2;
 
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
		
	}
}




int main(int argc, char** argv) {
  while(1) {
    printf("1");
    fflush(stdout);
    BIO_do_accept( bioStack );
    printf("2");
    fflush(stdout);
    bio = BIO_pop( bioStack );
    printf("3");
    fflush(stdout);
    if (bio != NULL && BIO_set_nbio(bio, 1) == 1) {
      vBio.push_back(bio);
    }
    else if (bio == NULL) {
      printf("NULO\n");
    fflush(stdout);
    }
    else {
      printf("Bloquante\n");
    fflush(stdout);
    }

    printf("4");
    fflush(stdout);
    for (int i = 0; i < vBio.size(); ++i) {
    printf("B");
      rvd = BIO_read(vBio[i], buf, len);
      if (rvd > 0) {
	printf("Recibido: %s\n", buf);
	BIO_write(bio, buf, rvd);
      }
      else if (rvd == -1 && errno == EAGAIN) {
	printf("NADA\n");
      }
      if (rvd == 0) {
	printf("Nada\n");
      }
      else if(rvd == -1) {
	BIO_free( bio );
	vBio.erase(vBio.begin()+i);
      }
    }
  }
  
  printf("Bye\n");
  BIO_free_all(bio);
}



