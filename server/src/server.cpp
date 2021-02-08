#include "App.h"
#include "AsyncFileReader.h"
#include "AsyncFileStreamer.h"
#include <unordered_map>
#include <string>

#define MAX_GAMES 10
#define MAX_PLAYERS 10
#define MAX_NAME_LEN 10
#define NO_OWNER 0xFF
/*
namespace Msg {
  enum {
    CREATE = 0,
    JOIN,
    LEAVE,
    OWNER,
    START,
    ACT,
    UPDATE,
    NUM_TYPES
  };

  struct create {
    const char type = CREATE;
    char name[MAX_NAME_LEN];
  };

  struct join {
    const char type = JOIN;
    char name[MAX_NAME_LEN];
    uint8_t id;
  };

  typedef uint8_t leave;
  typedef uint8_t owner;
}

typedef struct {
  bool connected;
  uWS::WebSocket<false, true> * ws;
} Player;

typedef struct {
  char name[MAX_NAME_LEN];
  char passcode[MAX_NAME_LEN];
  Player * owner;
  Player players[MAX_PLAYERS];

  template <typename Data>
  void publish(Data && data) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
      // TODO: Should I do this without corking? It's only one send.
      if (this->players[i].connected) {
        this->players[i].ws->send(std::string_view((char *) &data, sizeof(data)));
      }
    }
  }
} Game;

struct WSData {
  Player * player;
  Game * game;
};



int main() {
  AsyncFileStreamer streamer = AsyncFileStreamer("../dist/");
  std::unordered_map<std::string_view, Game *> games;

  uWS::App()
  .get("/", [&streamer](auto *res, auto *) {

    streamer.streamFile(res, "index.html");

  }).get("/*", [&streamer](auto *res, auto *req) {
    
    if (!streamer.streamFile(res, req->getUrl().substr(1))) {
      res->writeStatus("404 Not Found");
      res->end("I could not find that page.");
    } else {
      std::cout << "Served " << req->getUrl() << std::endl;
    };

  }).ws<WSData>("/ws", {
    .compression = uWS::SHARED_COMPRESSOR,
    .maxPayloadLength = 1024,
    .idleTimeout = 60,
    .maxBackpressure = 1024 * 1024,
    .upgrade = [&games](auto *res, uWS::HttpRequest *req, auto *context) {
      std::cout << "Upgrade request: " << req->getUrl() << "?" << req->getQuery() << std::endl;
      WSData data;

      std::string_view code = req->getQuery("j");
      std::cout << "Code: " << code << std::endl;
      if (code.data() != nullptr && code.length() > 0 && code.length() <= MAX_NAME_LEN) {
        auto iter = games.find(code);
        if (iter != games.end()) {
          auto game = iter->second;
          std::cout << "Checking pass" << std::endl;
          auto query = req->getQuery("p");

          if ((query.data() == nullptr && game->passcode[0] == 0) 
            || (query == game->passcode)) {
            for (int i = 0; i < MAX_PLAYERS; i++) {
              if (!game->players[i].connected) {
                game->players[i].connected = true;
                data.player = &game->players[i];
                data.game = game;
                std::cout << "New player added to game " << game->name << ": " << i << std::endl;
                goto upgrade;
              }
            }
            res->writeStatus("400 Bad Request")->end("Game is full.");
            std::cout << "GAME FULL" << std::endl;
            return;
          } else {
            
            res->writeStatus("401 Unauthorized")->end("Invalid Passcode.");
            if (query.data() == nullptr) {
              std::cout << "No password" << std::endl;
            } else {
              std::cout << "Password provided: " << query << " expected: " << game->passcode << std::endl;
            }
            return;
          }
        } else {
          res->writeStatus("404 Not Found")->end("Game not found.");
          std::cout << "GAME NOT FOUND" << std::endl;
          return;
        }
      }

      code = req->getQuery("c");
      if (code.data() != nullptr && code.length() > 0 && code.length() <= MAX_NAME_LEN) {
        auto iter = games.find(code);
        if (iter == games.end()) {
          Game * game = new Game();
          code.copy((char *)game->name, code.length(), 0);
          code = req->getQuery("p");
          if (code.data() != nullptr) {
            code.copy((char *)game->passcode, code.length(), 0);
          }
          game->players[0].connected = 1;
          game->owner = game->players;
          games.insert({game->name, game});
          data.player = &game->players[0];
          data.game = game;

          std::cout << "New game " << game->name << ", passcode: " << game->passcode << std::endl;
          goto upgrade;
        } else {
          res->writeStatus("400 Bad Request")->end("Game already exists.");
          return;
        }
      }

      res->writeStatus("404 Not Found")->end("Invalid request.");
      return;

    upgrade:
      res->template upgrade<WSData>(std::move(data), 
          req->getHeader("sec-websocket-key"),
          req->getHeader("sec-websocket-protocol"),
          req->getHeader("sec-websocket-extensions"),
          context);
    },
    .open = [](auto *ws) {
      WSData * data = (WSData *) ws->getUserData();
      if (data->player->connected) {
        data->player->ws = ws;
        data->game->publish(Msg::join {
          .name = "Player?--",
          .id = 0
        });

        std::cout << "Connection complete to game " << data->game->name << std::endl;
      } else {
        ws->close();
      }



      std::cout << "Connection opened" << std::endl;
    },
    .message = [&games](uWS::WebSocket<false,true> *, std::string_view message, uWS::OpCode) {
      char type = message.at(0);
      switch (type) {
        case Msg::CREATE:
          //Msg::create msg;
          //message.copy((char *)&msg, sizeof(msg), 1);
          break;
        case Msg::JOIN:
        case Msg::LEAVE:
        case Msg::ACT:
        default:
          std::cout << "Unknown command" << std::endl;
          break;
      }
      
      std::cout << message << std::endl;
    },
    .close = [](auto *ws, int, std::string_view) {
      WSData * data = (WSData *) ws->getUserData();

      data->player->connected = false;

      if (data->player == data->game->owner) {
        // if this player was the owner, then set the next in line to the owner
        for (int i = 0; i < MAX_PLAYERS; i++) {
          if (data->game->players[i].connected) {
            data->game->owner = &data->game->players[i];
          }
        }
      }
      
      std::cout << "Connection closed" << std::endl;
    }
  }).listen(4000, [](auto *listen_socket) {
    if (listen_socket) {
      std::cout << "Listening on 4000" << std::endl;
    } else {
      std::cout << "Error... Is the port busy?" << std::endl;
    }
  }).run();
}*/