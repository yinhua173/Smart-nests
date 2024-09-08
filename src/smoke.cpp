#include "smoke.h"
begin smoke;
begin fire;
begin rain;
begin pir;
begin touch;
void smokeTask(void *parameter) {//模拟信号
  smoke.init(17,27);
  while (true) {
    smoke.Arun(analogRead(17),27);
  }
}
void fireTask(void *parameter) {//模拟信号
  fire.init(16,26);
  while (true) {
    fire.Arun(analogRead(16),26);
  }
}
void rainTask(void *parameter) {//数字信号
  rain.init(18,25);
  while (true) {
    rain.run(18,25);
  }
}
void pirTask(void *parameter) {//数字信号
  pir.init(23,33);
  while (true) {
    pir.run(23,33);
  }
}
void touchTask(void *parameter) {//数字信号
  touch.init(19,32);
  while (true) {
    touch.run(19,32);
  }
}