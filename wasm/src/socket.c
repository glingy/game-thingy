#include "structs.h"

SocketMsg outMsg = {0};
SocketMsg inMsg = {0};

export int msgLen() {
  return sizeof(SocketMsg);
} 

extern void print(int msg);
extern void send(void);



export void recvMessage() {
  print(inMsg.tick);
}

export int testSend(int tick) {
  outMsg.tick = tick;
  send();
  return tick;
}