#include "smoke.h"
begin smoke;
begin fire;
begin rain;
begin pir;
begin touch;
void smokeTask(void *parameter) {//模拟信号
  smoke.init(17,27);
  while (1) {
    //smoke.value=analogRead(17);
    //smoke.Arun(smoke.value,27);
  }
}
void fireTask(void *parameter) {//模拟信号
  fire.init(16,26);
  while (1) {
    fire.value=analogRead(16);
    fire.Arun(fire.value,26);
  }
}
void rainTask(void *parameter) {//数字信号
  rain.init(18,25);
  while (1) {
    rain.run(18,25);
  }
}
void pirTask(void *parameter) {//数字信号
  pir.init(23,33);
  while (1) {
    pir.run(23,33);
  }
}
void touchTask(void *parameter) {//数字信号
  touch.init(19,32);
  while (1) {
    touch.run(19,32);
  }
}