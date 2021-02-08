#include "structs.h"

#define ACTION_DEF(enum_name) void action##enum_name(SocketMsg * msg, World * world)
#define DELTA_DEF(enum_name) void delta##enum_name(SocketMsg * msg, World * world)

#define USE(action) __attribute__((weak)) ACTION_DEF(action) {}
ACTIONS

#define USE(delta) __attribute__((weak)) DELTA_DEF(delta) {}
DELTAS

#define USE(action_enum) action##action_enum,
const ActionFn actions[NUM_ACTIONS] = {
  ACTIONS
};

#define USE(delta_enum) delta##delta_enum,
const DeltaFn deltas[NUM_ACTIONS] = {
  DELTAS
};

