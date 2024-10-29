#include "smoke.h"
begin smoke;
begin fire;
begin rain;
begin pir;
begin touch;
begin door;
begin win;
void smokeTask(void *parameter) {//模拟信号
  smoke.init(34);
  while (1) {
    smoke.value=analogRead(34)*(5000/4023);
    smoke.Arun(smoke.value);
    vTaskDelay(100);
  }
}
void fireTask(void *parameter) {//数字信号
  fire.init(35);
  while (1) {
    fire.run(35);
    vTaskDelay(100);
  }
}
void rainTask(void *parameter) {//数字信号
  rain.init(39);
  while (1) {
    rain.run(39);
    vTaskDelay(100);
  }
}
void pirTask(void *parameter) {//数字信号
  pir.init(36);
  while (1) {
    pir.run(36);
    vTaskDelay(100);
  }
}
void touchTask(void *parameter) {//数字信号
  touch.init(19);
  while (1) {
    touch.run(19);
    vTaskDelay(100);
  }
}
void doorTask(void *parameter) {//数字信号
  door.init(19);
  while (1) {
    door.run(19);
    vTaskDelay(100);
  }
}
void winTask(void *parameter) {//数字信号
  win.init(19);
  while (1) {
    win.run(19);
    vTaskDelay(100);
  }
}
void smokeprintTask(void *parameter) {
  while (1) {
    Serial.print("smoke:");
    Serial.print(smoke.status);
    Serial.print("  fire:");
    Serial.print(fire.status);
    Serial.print("  rain:");
    Serial.print(rain.status);
    Serial.print("  pir:");
    Serial.print(pir.status);
    Serial.print("  touch:");
    Serial.print(touch.status);
    Serial.print("  door:");
    Serial.print(door.status);
    Serial.print("  win:");
    Serial.println(win.status);
    vTaskDelay(1000);
  }
}