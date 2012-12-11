#include<iostream>	//cout
#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<string>	//string
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include<netdb.h>	//hostent

int sysConnect(int sock,sockaddr* echoserver, unsigned int size){
	return connect(sock,echoserver,size);
}

int sysSend(int& a, char *b, int& c, int d){
	return send(a,b,c,d);
}
