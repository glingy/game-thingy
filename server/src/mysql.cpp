#include "mysql.hpp"
#include <stdio.h>
#include <iostream>
#include <mysql/mysql.h>
#include ".env.h"

MYSQL * mysql;

SQL::SQL() {
  mysql = mysql_init(NULL);
  mysql_real_connect(mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, 3306, 0, 0);
}

SQL::~SQL() {
  mysql_close(mysql);
}


int main() {
  SQL sql;

  //mysql_query(mysql, "SHOW TABLES");
  //MYSQL_RES res;
  //mysql_fetch_row(&res);
  //std::cout << res.data << std::endl;
  std::cout << mysql_get_client_info() << std::endl;
  std::cout << "Something worked!" << std::endl;
}