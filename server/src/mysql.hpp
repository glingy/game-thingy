#include <mysqlx/xdevapi.h>

class SQL {
  public:
  SQL();
  ~SQL();
  static mysqlx::Schema * player;
  static mysqlx::Schema * user;

  private:
  static mysqlx::Session * session; 
};