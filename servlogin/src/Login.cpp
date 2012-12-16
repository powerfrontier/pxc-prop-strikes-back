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
    managerControl->listen(CONTROL_PORT);    
    managerClient->setMyClose(clientClosedConnection);
    managerClient->listen(CLIENT_PORT);
    
}

void Login::initializeLogin(){
    clientClosedConnection = new ClientClosedConnection();
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

bool Login::validate(string user, string pwd){

  MYSQL_RES *result;
  MYSQL_ROW row;
  int num_fields, num_rows;
  int i;
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

