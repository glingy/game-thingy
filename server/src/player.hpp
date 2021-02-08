#include <string>

class Player {
  unsigned long user_id;
  bool is_bot;
  unsigned long rank;
  unsigned long games_played;
  unsigned long games_won;
  std::string name;


  public:
  /**
   * Bot player from auth token
   */
  Player(std::string_view name);

  /**
   * Registered user player from id.
   * Assumes auth already took place.
   */
  Player(unsigned long id);

  /**
   * Guest player
   */
  Player();

  static Player Bot(std::string_view bot_auth_token);

  
};