#include <iostream>
#include "DatabaseConnection.h"

MYSQL* conn; // Define the global variable for MySQL connection

void db_response::ConnectionFunction() {
    if (mysql_library_init(0, NULL, NULL)) {
        std::cout << "Could not initialize MySQL library" << std::endl;
        return;
    }

    conn = mysql_init(0);
    if (!conn) {
        std::cout << "MySQL Initialization Failed" << std::endl;
        mysql_library_end();  // Only end the library if the initialization fails
        return;
    }

    conn = mysql_real_connect(conn, "localhost", "root", "", "fit", 3306, NULL, 0);
    if (conn) {
        std::cout << "Database Connected To MySQL" << std::endl;
    }
    else {
        std::cout << "Failed to connect to MySQL! Error: " << mysql_error(conn) << std::endl;
        mysql_library_end();  // End the library here if the connection fails
    }
}

void db_response::Cleanup() {
    if (conn) {
        mysql_close(conn);  // Close the database connection
    }
    mysql_library_end();  // End the MySQL library (done after the connection is closed)
}
