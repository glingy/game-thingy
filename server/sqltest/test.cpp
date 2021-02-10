#include <mysql/mysql.h>
#include <iostream>
#include ".env.h"

int main(void) {
  MYSQL mysql = { 0 }; 
  mysql_real_connect(&mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, 3306, NULL, 0);
  std::cout << "Hello world!" << std::endl;
  mysql_close(&mysql);
  return 0;
}




// EE 333 - projects
