#include "common.h"
#include "ssl.h"
#include "client.h"
#include "read_write.h"
#include <stdio.h>
#include <string.h>


static char *host=HOST;
static int port=PORT;

int main(int argc, char** argv) {
	SSL_CTX *ctx;
    	SSL *ssl;
	BIO *sbio;
	int sock;

	//extern char *optarg;
 	//int c;

	/* Build our SSL context*/
	ctx=initialize_ctx(KEYFILE,PASSWORD);

	//cache mirar como funciona y si lo podemos aprovechar para pasar de server a server
//	SSL_CTX_set_session_id_context(ctx,(void*)&s_server_session_id_context,sizeof s_server_session_id_context); 

	/* Connect the TCP socket*/
	sock=tcp_connect(host,port);

	/* Connect the SSL socket */
	ssl=SSL_new(ctx);
	sbio=BIO_new_socket(sock,BIO_NOCLOSE);
	SSL_set_bio(ssl,sbio,sbio);

	if(SSL_connect(ssl)<=0)
		berr_exit("SSL connect error");

	check_cert(ssl,host);

	/* read and write */
	read_write(ssl,sock);

	destroy_ctx(ctx);
	close(sock);


	exit(0);
} 	

