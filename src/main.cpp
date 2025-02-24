#include <Arduino.h>
#include <Wire.h>
#include "smoke.h"
#include "BH_1750.h"
#include "BME680.h"
#include "wifi_connect.h"
#include "fingermain.h"
#include "button.h"
#include "display_menu.h"
#include "aliyun.h"
#include "http_api.h"
#include "datadata.h"
#include "fingerdata.h"
#include "D74HC595.h"
//#include "TTimer.h"
#include "D3231.h"
// //任务控制权柄
// extern TaskHandle_t xHandleTsak[4];
extern TaskHandle_t wifiHandle;
extern TaskHandle_t aliyunHandle;
extern TaskHandle_t smokeHandle;
extern TaskHandle_t fireHandle;
extern TaskHandle_t rainHandle;
extern TaskHandle_t pirHandle;
extern TaskHandle_t touchHandle;
//extern TaskHandle_t smokeprintHandle = NULL;
extern TaskHandle_t winHandle;
extern TaskHandle_t doorHandle;
extern TaskHandle_t bh1750Handle;
extern TaskHandle_t bme680Handle;
extern TaskHandle_t httpHandle;
extern TaskHandle_t d3231Handle;
extern TaskHandle_t tof200Handle;
extern TaskHandle_t FingerHandle;
extern TaskHandle_t oledHandle;
extern TaskHandle_t datadataHandle;
extern TaskHandle_t D74HC595Handle;
// // 定时器控制权柄
// extern TimerHandle_t xTimeHandle[2];
// TaskHandle_t taskHandle;
// int taskMem = 1024*90;
/*
Total Heap Size:  317776 Bytes
Free Heap Size:  115064 Bytes
wifitask    task free memory:  2108       task used memory:  1988
httpTask    task free memory:  34792      task used memory:  67608
aliyunTask  task free memory:  2208       task used memory:  1888
ledTask     task free memory:  804        task used memory:  1244
smokeTask   task free memory:  264        task used memory:  760
fireTask    task free memory:  1288       task used memory:  760
rainTask    task free memory:  480        task used memory:  544
pirTask     task free memory:  436        task used memory:  588
touchTask   task free memory:  380        task used memory:  644
BH1750Task  task free memory:  1008       task used memory:  1040
bme680Task  task free memory:  1444       task used memory:  1628
fingerTask  task free memory:  876        task used memory:  1172
OLEDTask    task free memory:  2916       task used memory:  1180
*/
void setup() {
  Serial.begin(115200);
  // creat();
  //xTimerStart(xTimeHandle[0], 0);
  xTaskCreatePinnedToCore(wifiTask, "wifiTask", 1024 * 4, NULL, 1, &wifiHandle, 0);//创建wifi任务
  xTaskCreatePinnedToCore(aliyunTask, "aliyunTask", 1024 * 4, NULL, 2, &aliyunHandle, 0);//创建阿里云任务
  
  xTaskCreatePinnedToCore(smokeTask, "smokeTask", 1024*2, NULL, 2, &smokeHandle, 1);//创建烟雾任务
  xTaskCreatePinnedToCore(fireTask, "fireTask", 1024*2, NULL, 2, &fireHandle, 1);//创建火焰任务
  xTaskCreatePinnedToCore(rainTask, "rainTask", 1024*2, NULL, 2, &rainHandle, 1);//创建雨滴任务
  xTaskCreatePinnedToCore(pirTask, "pirTask", 1024*2, NULL, 2, &pirHandle, 1);//创建人体红外任务
  xTaskCreatePinnedToCore(touchTask, "touchTask", 1024*2, NULL, 2, &touchHandle, 1);//创建触摸任务
  xTaskCreatePinnedToCore(doorTask, "doorTask", 1024 * 2, NULL, 2, &doorHandle, 1);//创建门口任务
  xTaskCreatePinnedToCore(winTask, "winTask", 1024 * 2, NULL, 2, &winHandle, 1);//创建窗户任务

  xTaskCreatePinnedToCore(BH1750Task, "BH1750Task", 1024 * 2, NULL, 4, &bh1750Handle, 1);//创建光照任务
  xTaskCreatePinnedToCore(bme680Task, "bme680Task", 1024 * 3, NULL, 4, &bme680Handle,1);//创建温湿度任务
  
  xTaskCreatePinnedToCore(httpTask, "httpTask", 1024*85, NULL, 3, &httpHandle, 1);//创建http任务

  xTaskCreatePinnedToCore(D3231Task, "D3231Task", 1024*5, NULL, 1, &d3231Handle, 1);//创建RTC任务
  xTaskCreatePinnedToCore(TOF200Task, "TOF200Task", 1024*5, NULL, 1, &tof200Handle, 1);//创建TOF任务

  xTaskCreatePinnedToCore(fingerTask, "fingerTask", 1024 * 3, NULL, 1, &FingerHandle,1);//创建指纹任务
  xTaskCreatePinnedToCore(OLEDTask, "OLEDTask", 1024*10, NULL, 3, &oledHandle, 0);//创建OLED任务
  xTaskCreatePinnedToCore(datadata_task, "datadata_task", 1024 * 20, NULL, 2, &datadataHandle,1);//数据包任务
  
  xTaskCreatePinnedToCore(D74HC595Task, "D74HC595Task", 1024*4, NULL, 1, &D74HC595Handle, 1);//创建控制端任务
  buttonTask();
  // xTaskCreatePinnedToCore(delay_test_task, "delay_test_task", 1024 * 4, NULL, 1, NULL,1);//给指纹的判断任务
  //vTaskDelay(1000); //提前先运行一秒获取第一笔数据

  //xTaskCreatePinnedToCore(smokeprintTask, "smokeprintTask", 1024 * 4, NULL, 1, NULL,1);//创建打印任务
  //xTaskCreatePinnedToCore(printTask, "printTask", 1024 * 4, NULL, 4, NULL,1);//创建打印任务

  // int heapSize = ESP.getHeapSize();
  // Serial.print("Total Heap Size:  ");
  // Serial.print(heapSize);
  // Serial.println(" Bytes");

  // int heapFree = ESP.getFreeHeap();
  // Serial.print("Free Heap Size:  ");
  // Serial.print(heapFree);
  // Serial.println(" Bytes");
  // Serial.println("");

  // Serial.println("Create Task ...");
  // xTaskCreatePinnedToCore(httpTask, "httpTask", 1024*90, NULL, 3, &taskHandle, 1);//创建http任务
  // // xTaskCreatePinnedToCore(fingerTask, "fingerTask", taskMem, NULL, 1, &taskHandle,1);

  // Serial.print("Free Heap Size:  ");
  // Serial.print(ESP.getFreeHeap());
  // Serial.println(" Bytes");
  // Serial.println("");

  // vTaskDelay(60000*3);
  // int waterMark = uxTaskGetStackHighWaterMark(taskHandle);
  // Serial.print("Task Free Memory: ");
  // Serial.print(waterMark);
  // Serial.println(" Bytes");
  // Serial.print("Task Used Memory: ");
  // Serial.print(taskMem - waterMark);
  // Serial.println(" Bytes");
  vTaskDelete(NULL); //删除setup函数创建的空闲任务
}
// void vApplicationleHook(void){

// }
void loop() {
}
