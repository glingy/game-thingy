#include "mysql.hpp"
#include <stdio.h>
#include <iostream>


using namespace mysqlx;

Session * SQL::session;
Schema * SQL::player;
Schema * SQL::user;

SQL::SQL() {
  session = new Session("localhost", 3306, "root", "root");
  user = new Schema(*session, "user");
  player = new Schema(*session, "player");

  Collection users = user->getCollection("test_user", true);
  users.add(R"({ "name":"Test User", "email":":param" })").execute();
}

SQL::~SQL() {
  delete player;
  delete user;
  delete session;
}


int main() {
  SQL sql;
  std::cout << "Something worked!" << std::endl;
}