#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <mysql.h>

// Declare the global variable for MySQL connection
extern MYSQL* conn;

class db_response {
public:
    // Establishes the MySQL connection
    static void ConnectionFunction();

    // Cleans up and closes the MySQL connection
    static void Cleanup();
};

#endif // DATABASECONNECTION_H
