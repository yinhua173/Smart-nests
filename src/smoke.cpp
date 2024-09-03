#include "smoke.h"
begin smoke;
begin fire;
begin rain;
begin pir;
begin touch;
void smokeTask(void *parameter) {
  smoke.init(2,3);
  while (true) {
    smoke.run(2,3);
  }
}
void fireTask(void *parameter) {
  fire.init(4,5);
  while (true) {
    fire.run(4,5);
  }
}
void rainTask(void *parameter) {
  rain.init(6,7);
  while (true) {
    rain.run(6,7);
  }
}
void pirTask(void *parameter) {
  pir.init(8,9);
  while (true) {
    pir.run(8,9);
  }
}
void touchTask(void *parameter) {
  touch.init(10,11);
  while (true) {
    touch.run(10,11);
  }
}