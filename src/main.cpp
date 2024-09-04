#include <Arduino.h>
#include <Wire.h>
#include "smoke.h"
#include "BH_1750.h"
#include "BME680.h"

void setup() {
  xTaskCreatePinnedToCore(smokeTask, "lightTask", 1024 * 8, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(BH1750Task, "BH1750Task", 1024 * 8, NULL, 1, NULL, 1);
  xTaskCreate(bme680Task, "BME680", 1024 * 8, NULL, 1, NULL);
  vTaskDelay(1000); //让MPU6050提前先运行一秒获取第一笔数据
  xTaskCreate(printTask, "print", 1024 * 8, NULL, 1, NULL);
}

void loop() {
}
