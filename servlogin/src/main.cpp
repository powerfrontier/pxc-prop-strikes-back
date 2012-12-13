#include <Login.h>
#include <LoginProfile.h>
#include <iostream>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include <mysql_connection.h>
#include <mysql_driver.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;




int main() {
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new LoginProfile());
	TransferableFactory::instance().setProtocol("0.1a");
	Login::instance().initializeManager();
	printf("Login: Estoy en listen\n");
 	fflush(stdout);
	/*try {
	sql::mysql::MySQL_Driver *driver;
	//sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	
	/* Create a connection */
	/*driver = sql::mysql::get_driver_instance();*/
	//con = driver->connect("tcp://mysqlfib.fib.upc.es:3306", "BDpxc03", "BDpxc03");
	/* Connect to the MySQL test database */
	//con->setSchema("test");

	//stmt = con->createStatement();
	//res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
	//while (res->next()) {
	  //cout << "\t... MySQL replies: ";
	  /* Access column data by alias or column name */
	  //cout << res->getString("_message") << endl;
	  //cout << "\t... MySQL says it again: ";
	  /* Access column fata by numeric offset, 1 is the first column */
	  //cout << res->getString(1) << endl;
	//}
	/*delete res;
	delete stmt;
	delete con;
	
	
	}catch(sql::SQLException &e) {
	    cout << "# ERR: SQLException in " << __FILE__;
	    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
	    cout << "# ERR: " << e.what();
	    cout << " (MySQL error code: " << e.getErrorCode();
	    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}*/
	while (1){sleep(50);}
}
