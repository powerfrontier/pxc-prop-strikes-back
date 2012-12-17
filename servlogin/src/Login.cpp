#include <Login.h>
#include <LoginProfile.h>
#include <iostream>
#include <mysql.h>
#include <ClientClosedConnection.h>
#include <Connection.h>
#include <sstream>

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

string Login::convertPwdToSha1(string pwd, int lengthPwd){
   	unsigned char ibuf[SHA_DIGEST_LENGTH*2];
	unsigned char obuf[SHA_DIGEST_LENGTH*2];	
	int i;	
	strcpy((char*) ibuf,pwd.c_str());	
	SHA1(ibuf, lengthPwd, obuf);
	char resultPwd[SHA_DIGEST_LENGTH*2];
	memset(resultPwd, 0x0, SHA_DIGEST_LENGTH);
	for (i=0; i < SHA_DIGEST_LENGTH; i++) {
		sprintf((char*)&(resultPwd[i*2]), "%02x", obuf[i]);
	}
    	return resultPwd;
}


bool Login::validate(string user, string pwd){
  const char* resultPwd;
  string passwordCod;
  MYSQL_RES *result;
  MYSQL_ROW row;
  int num_fields, num_rows;
  int i;
  pwd = Login::instance().convertPwdToSha1(pwd,static_cast< int >( strlen( user.c_str() ) ));
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
  return num_rows;
	
}



Login::~Login(){
  mysql_close(mysqlConnection);
  delete managerClient;
  delete managerControl;
}

