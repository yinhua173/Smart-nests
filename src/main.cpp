#include <Arduino.h>
#include <Wire.h>
#include "smoke.h"
#include "BH_1750.h"
#include "BME680.h"
#include "wifi_connect.h"

//TaskHandle_t taskHandle;
//int taskMem = 1024*3;

void setup() {
  Serial.begin(9600);
  xTaskCreatePinnedToCore(wifiTask, "wifiTask", 1024 * 4, NULL, 1, NULL, 1);//创建wifi任务
  xTaskCreatePinnedToCore(smokeTask, "smokeTask", 1024, NULL, 1, NULL, 1);//创建烟雾任务
  xTaskCreatePinnedToCore(fireTask, "fireTask", 1024, NULL, 1, NULL, 1);//创建火焰任务
  xTaskCreatePinnedToCore(rainTask, "rainTask", 1024, NULL, 1, NULL, 1);//创建雨滴任务
  xTaskCreatePinnedToCore(pirTask, "pirTask", 1024, NULL, 1, NULL, 1);//创建人体红外任务
  xTaskCreatePinnedToCore(touchTask, "touchTask", 1024, NULL, 1, NULL, 1);//创建触摸任务
  xTaskCreatePinnedToCore(BH1750Task, "BH1750Task", 1024 * 2, NULL, 1, NULL, 1);//创建光照任务
  xTaskCreatePinnedToCore(bme680Task, "bme680Task", 1024 * 2, NULL, 1, NULL,1);//创建温湿度任务
  vTaskDelay(1000); //让MPU6050提前先运行一秒获取第一笔数据
  xTaskCreatePinnedToCore(printTask, "print", 1024 * 3, NULL, 1, NULL,1);//创建打印任务

/*   int heapSize = ESP.getHeapSize();
  Serial.print("Total Heap Size:  ");
  Serial.print(heapSize);
  Serial.println(" Bytes");

  int heapFree = ESP.getFreeHeap();
  Serial.print("Free Heap Size:  ");
  Serial.print(heapFree);
  Serial.println(" Bytes");
  Serial.println("");


  Serial.println("Create Task ...");
  //xTaskCreate(wifiTask, "wifiTask", taskMem*4, NULL, 1, &taskHandle);

  Serial.print("Free Heap Size:  ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" Bytes");
  Serial.println("");

  vTaskDelay(2000);
  int waterMark = uxTaskGetStackHighWaterMark(taskHandle);
  Serial.print("Task Free Memory: ");
  Serial.print(waterMark);
  Serial.println(" Bytes");
  Serial.print("Task Used Memory: ");
  Serial.print(taskMem - waterMark);
  Serial.println(" Bytes"); */
  vTaskDelete(NULL); //删除setup函数创建的空闲任务
}

void loop() {
}
