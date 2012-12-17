#include <Login.h>
#include <LoginProfile.h>
#include <iostream>
#include <mysql.h>
#include <ClientClosedConnection.h>
#include <Connection.h>

using namespace std;

void Login::initializeManager(){
 
    managerControl = new ConnectionManager();
    managerClient = new ConnectionManager();
    managerControl->listenSecure(CONTROL_PORT,true);    
    managerClient->setMyClose(clientClosedConnection);
    managerClient->listenSecure(CLIENT_PORT,false);
    
}

void Login::initializeLogin(){
    clientClosedConnection = new ClientClosedConnection();
    controlConnected = false;
    my_bool reconnect = 1;
    nextFreeToken = 0;
    usersConnected = 0;
    mysqlConnection = mysql_init(NULL);
    mysql_options(mysqlConnection, MYSQL_OPT_RECONNECT, &reconnect);
    if(mysql_real_connect(mysqlConnection, DBHOSTNAME, DBUSER, DBPASSWORD, DBNAME, 3306, NULL, 0) == NULL){
      fprintf(stderr, "Failed to connect to database: Error: %s\n",
      mysql_error(mysqlConnection));
    } 
}

unsigned char* Login::convertPwdToSha1(unsigned char* pwdOrig, int lengthPwd){
    unsigned char* pwdCrypt;
    SHA1(pwdOrig, lengthPwd, pwdCrypt);

    int i;
    for (i = 0; i < lengthPwd; i++) {
        printf("%02x ", pwdCrypt[i]);
    }
    printf("\n"); 
    return pwdCrypt;
}



bool Login::validate(string user, string pwd){
  const char* resultPwd;
  MYSQL_RES *result;
  MYSQL_ROW row;
  int num_fields, num_rows;
  int i;
  //resultPwd = pwd.c_str();
  //Login::instance().convertPwdToSha1((unsigned char*)resultPwd,user.length());
  cout << "user i password enviats: " << user << " " << pwd << endl;
  string query = "SELECT * FROM USERS WHERE USERNAME='" + user + "' AND PASSWORD='" + pwd +"'" ;
  mysql_query(mysqlConnection, query.c_str());
  result = mysql_store_result(mysqlConnection);
  num_fields = mysql_num_fields(result);
  num_rows = mysql_num_rows(result);
  while ((row = mysql_fetch_row(result)))
  {
      for(i = 0; i < num_fields; i++)
      {
          printf("%s ", row[i] ? row[i] : "NULL");
      }
      printf("\n");
  }
  mysql_free_result(result);
  //delete result;
  return num_rows;
	
}



Login::~Login(){
  mysql_close(mysqlConnection);
  delete managerClient;
  delete managerControl;
}

