#include <Login.h>
#include <LoginProfile.h>
#include <iostream>


void Login::initializeManager(){
    manager = new ConnectionManager();
    manager->listen(CONTROL_PORT);
    manager->listen(CLIENT_PORT);
}

Login::~Login(){
  delete manager;
}

