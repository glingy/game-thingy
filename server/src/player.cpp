#include "player.hpp"

Player Player::Bot(std::string_view bot_auth_token) {
  return Player(bot_auth_token);
}

Player::Player(unsigned long id) {

}

Player::Player(std::string_view name): 
  user_id(0),
  is_bot(0),
  rank(0),
  games_played(0),
  games_won(0)
{
  this->name = std::string{name};
  this->user_id = 0;
}