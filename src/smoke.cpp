#include "smoke.h"
begin smoke;
begin fire;
begin rain;
begin pir;
begin touch;
void smokeTask(void *parameter) {//模拟信号
  smoke.init(34,2);
  while (1) {
    smoke.value=analogRead(34)*(5000/4023);
    smoke.Arun(smoke.value,2);
  }
}
void fireTask(void *parameter) {//模拟信号
  fire.init(35,5);
  while (1) {
    fire.value=analogRead(35)*(5000/4023);
    fire.Arun(fire.value,5);
  }
}
void rainTask(void *parameter) {//数字信号
  rain.init(15,13);
  while (1) {
    rain.run(15,13);
  }
}
void pirTask(void *parameter) {//数字信号
  pir.init(4,14);
  while (1) {
    pir.run(4,14);
  }
}
void touchTask(void *parameter) {//数字信号
  touch.init(19,25);
  while (1) {
    touch.run(19,25);
  }
}