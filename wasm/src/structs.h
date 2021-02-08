#include "actions.h"
#define export __attribute__((visibility( "default" ), used)) 


typedef struct {

} Player;

typedef struct {
  unsigned char tick;
} SocketMsg;

typedef struct {

} World;

typedef void (*ActionFn)(SocketMsg *, World *);
typedef void (*DeltaFn)(SocketMsg *, World *);


#define USE(name) name,
enum Actions {
  ACTIONS
  NUM_ACTIONS
};

enum Deltas {
  DELTAS
  NUM_DELTAS
};

extern const ActionFn actions[NUM_ACTIONS];
extern const DeltaFn deltas[NUM_ACTIONS];
extern SocketMsg lastMsg;
