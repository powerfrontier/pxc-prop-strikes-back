#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include<sysConnect.h>

int sysConnect(int sock,sockaddr* echoserver, unsigned int size){
	return connect(sock,(struct sockaddr *) &echoserver,size);
}

int sysSend(int& a, char *b, int& c, int d){
	return send(a,b,c,d);
}
