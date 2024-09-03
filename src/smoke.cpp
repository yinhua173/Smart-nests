#include "smoke.h"

void smokeTask(void *parameter) {
  smoke.init(2,3);
  while (true) {
    smoke.run(2,3);
  }
}
void fireTask(void *parameter) {
  fire.init(2,3);
  while (true) {
    fire.run(2,3);
  }
}
void rainTask(void *parameter) {
  rain.init(2,3);
  while (true) {
    rain.run(2,3);
  }
}
void pirTask(void *parameter) {
  pir.init(2,3);
  while (true) {
    pir.run(2,3);
  }
}
void touchTask(void *parameter) {
  touch.init(2,3);
  while (true) {
    touch.run(2,3);
  }
}