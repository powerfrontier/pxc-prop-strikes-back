#include <mysql.h>
#include <stdio.h>
#include <iostream>

#define usuari "pxc03"
int main()
{
using namespace std;

  // --------------------------------------------------------------------
    // Connect to the database
    

    string hostName = "mysqlfib.fib.upc.es";
    string userId   = "pxc03";
    string password = "nJoW03Hi";
    string DB       = "BDpxc03";


  MYSQL *conn;
  MYSQL_RES *result;
  MYSQL_ROW row;
  int num_fields;
  int i;

  conn = mysql_init(NULL);
  if(mysql_real_connect(conn, hostName.c_str(), usuari, password.c_str(), DB.c_str(), 3306, NULL, 0) == NULL)
{
    fprintf(stderr, "Failed to connect to database: Error: %s\n",
          mysql_error(conn));
}
string user = "SELECT * FROM USERS WHERE USERNAME='USEsR1'";
  mysql_query(conn, user.c_str());
  result = mysql_store_result(conn);

  num_fields = mysql_num_rows(result);


	cout << "resultats " << num_fields << endl;

  while ((row = mysql_fetch_row(result)))
  {
      for(i = 0; i < num_fields; i++)
      {
          printf("%s ", row[i] ? row[i] : "NULL");
      }
      printf("\n");
  }

  mysql_free_result(result);
  mysql_close(conn);


}

//g++ -Wall -pedantic -Wno-long-long -O2 -I../mysqlDriver/include -c dummy.cpp
//g++ dummy.o -L../mysqlDriver/lib -lmysqlclient -lpthread -o dummy

