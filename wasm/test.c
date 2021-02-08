#define export __attribute__((visibility( "default" ), used)) 

extern void printMath(int a, int b, int c);

int state = 0;

export void add(int b) {
  printMath(state, b, state + b);
  state = state + b;
}

export void addLoop() {
  while (1) {
    add(1);
  }
}