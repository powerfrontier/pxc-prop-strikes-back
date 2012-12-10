#include "ConnectionManager.h"
#include <iostream>
#include <string>
#include <openssl/bio.h> // BIO objects for I/O
#include <openssl/ssl.h> // SSL and SSL_CTX for SSL connections
#include <openssl/err.h> // Error reporting
#include <sys/socket.h>
/**
 * Simple log function
 */
void slog2(char* message) {
    fprintf(stdout, "%s", message);
}

/**
 * Print SSL error details
 */
void print_ssl_error2(char* message, FILE* out) {
 
    fprintf(out, "%s", message);
    fprintf(out, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
    fprintf(out, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    ERR_print_errors_fp(out);
}

/**
 * Print SSL error details with inserted content
 */
void print_ssl_error_23(char* message, char* content, FILE* out) {
 
    fprintf(out, message, content);
    fprintf(out, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
    fprintf(out, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    ERR_print_errors_fp(out);
}


ConnectionManager::ConnectionManager() throw(){
    /* call the standard SSL init functions */
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    cCallB = NULL;
}

ConnectionManager::~ConnectionManager() throw(){
	//TODO: KILL THREADS
	cCallB = NULL;
	if (tListen != NULL){
		delete tListen;
		tListen = NULL;
	}
}

void ConnectionManager::listen(const std::string& port) throw(ConnectionException){
	std::thread *t = new std::thread(&ConnectionManager::listenThread, this, port);
	tListenN.push_back(t);
}

void ConnectionManager::listenSecure(const std::string& port) throw(ConnectionException){
	std::thread *t = new std::thread(&ConnectionManager::listenThread, this, port);
	tListenN.push_back(t);
}

void ConnectionManager::setCallbackFunction(ConnectionCallback* callB) throw(){
	cCallB = callB;
}

void ConnectionManager::listenThread(const std::string& port) throw(ConnectionException){
	BIO* bioStack, *cbio;
	std::string str = port;
	char buff[64];
	str.copy(buff, str.size(), 0);
	bioStack = BIO_new_accept(buff);
	if (bioStack == NULL) {
		char message[] = "ListenThread(): bioStack = NULL.\n";
 		print_ssl_error2(message, stdout);
	}
	/* First call to BIO_accept() sets up accept BIO */
	if(BIO_do_accept(bioStack) <= 0) {
		char message[] = "ListenThread(): bioStack <= 0.\n";
 		print_ssl_error2(message, stdout);
	}
	while (1){
		cbio = BIO_pop(bioStack);
		if (cbio != NULL){
			Connection *c = new TCPConnection(cbio, port);
			if (cCallB != NULL){
				c->setCallbackFunction(cCallB);
			}
			c->receive();
		}
		/* Wait for incoming connection */
		printf("ESPERANDO CONEXION\n");
		if(BIO_do_accept(bioStack) <= 0) {
        		char message[] = "bioStack <= 0 x2.\n";
	 		print_ssl_error2(message, stdout);
		}
		
	}
}

void ConnectionManager::listenThreadSecure(const std::string& port) throw(ConnectionException){
/*
//********************************************************************************************************************************
	// Initialize Winsock.
	//WSADATA wsadata;
	//int ret = WSAStartup(0x101, &wsadata);
	//if (ret != 0) {
	//	printf("WSAStartup() failed with: %d!\n", GetLastError());
	//	return;
	//}

	// Next we need to create a server socket.
	SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sockaddrin;
	// Internet socket
	sockaddrin.sin_family = AF_INET;
	// Accept any IP
	sockaddrin.sin_addr.s_addr = INADDR_ANY;
	// Use port 
	std::string str = port;
	char buff[str.length()];
	str.copy(buff, str.size(), 0);
	sockaddrin.sin_port = htons(buff.atoi());

	// Valid socket?
	if (server == INVALID_SOCKET) {
		printf("Error creating server socket!");
		return;
	}

	// Now bind to the port
	ret = bind(server, (sockaddr*) &(sockaddrin), sizeof(sockaddrin));
	if (ret != 0) {
		printf("Error binding to port!\n");
		return;
	}

	// Start listening for connections
	// Second param is max number of connections
	ret = listen(server, 50);
	if (ret != 0) {
		printf("Error listening for connections!\n");
		return;
	}

	// Set up to accept connections
	SOCKET client;
	sockaddr_in clientsockaddrin;
	int len = sizeof(clientsockaddrin);
	printf("Server ready to accept connections!\n");
	while (1) {
		// Block until a connection is ready
		client = accept(server, (sockaddr*) &clientsockaddrin, &len);
		printf("Connection recieved from %s!\n", inet_ntoa(clientsockaddrin.sin_addr));

		// Notice that we use server_method instead of client_method
		SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());

		/* The following is commented out. This sets up certificate verification.

		You will need to generate certificates, the root certificate authority file, the private key file, and the random file yourself.
      Google will help. The openssl executable created when you compiled OpenSSL can do all this.

      // Sets the default certificate password callback function. Read more under the Certificate Verification section.
      SSL_CTX_set_default_passwd_cb(ctx, password_callback);
      // Sets the certificate file to be used.
      SSL_CTX_use_certificate_file(ctx, "casigned.pem", SSL_FILETYPE_PEM);
      // Sets the private key file to be used.
      SSL_CTX_use_PrivateKey_file(ctx, "cakey.pem", SSL_FILETYPE_PEM);

      // Load trusted root authorities
      SSL_CTX_load_verify_locations(ctx, "rootcas.pem", 0);
      // Set the maximum depth to be used verifying certificates
      // Due to a bug, this is not enforced. The verify callback must enforce it.
      SSL_CTX_set_verify_depth(ctx, 1);
      // Set the certificate verification callback.
      SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, verify_callback);

      // Load the random file, read 1024 << 10 bytes, add to PRNG for entropy
      RAND_load_file("random.pem", 1024 << 10);

      End certificate verification setup.
      */
/*
		// We need to load the Diffie-Hellman key exchange parameters.
		BIO* bio = BIO_new_file("dh1024.pem", "r");
		// Did we get a handle to the file?
		if (bio == NULL) {
			printf("Couldn't open DH param file!\n");
			break;
		}

		// Read in the DH params.
		DH* ret = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
		// Free up the BIO object.
		BIO_free(bio);
		// Set up our SSL_CTX to use the DH parameters.
		if (SSL_CTX_set_tmp_dh(ctx, ret) < 0) {
			printf("Couldn't set DH parameters!\n");
			break;
		}

		// Now we need to generate a RSA key for use.
		// 1024-bit key. If you want to use something stronger, go ahead but it must be a power of 2. Upper limit should be 4096.
		RSA* rsa = RSA_generate_key(1024, RSA_F4, NULL, NULL);

		// Set up our SSL_CTX to use the generated RSA key.
		if (!SSL_CTX_set_tmp_rsa(ctx, rsa)) {
			printf("Couldn't set RSA key!\n");
			// We don't break out here because it's not a requirement for the RSA key to be set. It does help to have it.
		}
		// Free up the RSA structure.
		RSA_free(rsa);

		SSL_CTX_set_cipher_list(ctx, "ALL");
		// Set up our SSL object as before
		SSL* ssl = SSL_new(ctx);
		// Set up our BIO object to use the client socket
		BIO* sslclient = BIO_new_socket(client, BIO_NOCLOSE);
		// Set up our SSL object to use the BIO.
		SSL_set_bio(ssl, sslclient, sslclient);

		// Do SSL handshaking.
		int r = SSL_accept(ssl);
		// Something failed. Print out all the error information, since all of it may be relevant to the problem.
		if (r != 1) {
			printf("SSL_accept() returned %d\n", r);
			printf("Error in SSL_accept(): %d\n", SSL_get_error(ssl, r));
			char error[65535];
			ERR_error_string_n(ERR_get_error(), error, 65535);
			printf("Error: %s\n\n", error);
			ERR_print_errors(sslclient);
			int err = WSAGetLastError();
			printf("WSA: %d\n", err);
			// We failed to accept this client connection.
			// Ideally here you'll drop the connection and continue on.
			break;
		}*/

		/* Verify certificate
		if (SSL_get_verify_result(client) != X509_V_OK) {
			printf("Certificate failed verification!\n");
			// Ideally here you'll close this connection and continue on.
			break;
		}
		*/

	/*	// Further work using the SSL object here (IO operations, new threads, etc)
		Connection *c = new TCPConnectionSecurity(ssl, port);
		if (cCallB != NULL){
			c->setCallbackFunction(cCallB);
		}
		c->receive();		
	}*/
//********************************************************************************************************************************
}
