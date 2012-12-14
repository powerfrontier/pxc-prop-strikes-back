#include <mysql++.h>
#include <iostream>

int main()
{
using namespace mysqlpp;
using namespace std;

try
{
Connection* conn = new Connection("BDpxc03", "mysqlfib.fib.upc.edu", "pxc03", "BDpxc03",3306);
cout << "Conexió realitzada? " << conn->connected() << endl;
}
catch (ConnectionFailed& e)
{
cerr << "Exception: " << e.what() << endl;
}
}

//g++ -Wall -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++ -c dummy.cpp
//g++ dummy.o `mysql_config --libs` -lmysqlpp -o dummy

