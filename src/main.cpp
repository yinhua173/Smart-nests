#include <Arduino.h>
#include <Wire.h>
#include "smoke.h"
#include "BH_1750.h"
#include "BME680.h"

void setup() {
  xTaskCreatePinnedToCore(smokeTask, "lightTask", 1024 * 8, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(BH1750Task, "BH1750Task", 1024 * 8, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(bme680Task, "BME680", 1024 * 8, NULL, 1, NULL,1);
  vTaskDelay(1000); //让MPU6050提前先运行一秒获取第一笔数据
  xTaskCreatePinnedToCore(printTask, "print", 1024 * 8, NULL, 1, NULL,1);
  vTaskDelete(NULL); //删除setup函数创建的空闲任务
}

void loop() {
}
