#include <Login.h>
#include <LoginProfile.h>
#include <iostream>


void Login::initializeManager(){
    manager = new ConnectionManager();
    manager->listen(CONTROL_PORT);
    manager->listen(CLIENT_PORT);
}

void Login::initializeLogin(){
    nextFreeToken = 0;
    usersConnected = 0;
}

bool Login::validate(const char* user, const char* pwd){
    return true;
}



Login::~Login(){
  delete manager;
}

