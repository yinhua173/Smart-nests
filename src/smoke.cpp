#include "smoke.h"
TaskHandle_t smokeHandle = NULL;
TaskHandle_t fireHandle = NULL; 
TaskHandle_t rainHandle = NULL;
TaskHandle_t pirHandle = NULL;
TaskHandle_t touchHandle = NULL; 
TaskHandle_t smokeprintHandle = NULL;
TaskHandle_t winHandle = NULL;
TaskHandle_t doorHandle = NULL;
beginn smoke;//烟雾
beginn fire;//火焰
beginn rain;//雨水
beginn pir;//红外
beginn touch;//触摸
beginn door;//门磁
beginn win;//窗户
void smokeTask(void *parameter) {//模拟信号
  smoke.init(34);
  while (1) {
    smoke.value=analogRead(34)*(5000/4023);
    smoke.Arun(smoke.value);
    vTaskDelay(50);
  }
}
void fireTask(void *parameter) {//数字信号
  fire.init(35);
  while (1) {
    fire.run(35);
    vTaskDelay(50);
  }
}
void rainTask(void *parameter) {//数字信号
  rain.init(39);
  while (1) {
    rain.run(39);
    vTaskDelay(50);
  }
}
void pirTask(void *parameter) {//数字信号
  pir.init(36);
  while (1) {
    pir.run(36);
    vTaskDelay(500);
  }
}
void touchTask(void *parameter) {//数字信号
  touch.init(19);
  while (1) {
    touch.run(19);
    vTaskDelay(50);
  }
}
void doorTask(void *parameter) {//数字信号
  door.init(5);
  while (1) {
    door.run(5);
    vTaskDelay(50);
  }
}
void winTask(void *parameter) {//数字信号
  win.init(4);
  while (1) {
    win.run(4);
    vTaskDelay(50);
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