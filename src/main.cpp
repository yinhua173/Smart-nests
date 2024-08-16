#include <Arduino.h>
/*
  程序： 内存管理
  公众号：孤独的二进制
  API：
    ESP.getHeapSize() //本程序Heap最大尺寸
    ESP.getFreeHeap() //当前Free Heap最大尺寸
    uxTaskGetStackHighWaterMark(taskHandle) //Task内存使用最大水位线，内存是水
     
    What is the Highest Water Mark?
    the minimum amount of remaining stack space that was available to the task 
    since the task started executing - that is the amount of stack that remained 
    unused when the task stack was at its greatest (deepest) value. This is what 
    is referred to as the stack 'high water mark'.
*/
TaskHandle_t taskHandle;
int taskMem = 1024;

void task(void *ptParam) {
  //volatile char hello[1000] = {0}; //必须要用volatile修饰语，否则会被编译器优化掉
  while (1) {

    //不推荐在task中执行，因为Serial.print也会消耗内存
    // vTaskDelay(2000);
    // int waterMark = uxTaskGetStackHighWaterMark(nullptr);
    // Serial.print("Task Free Memory: ");
    // Serial.print(waterMark);
    // Serial.println(" Bytes");
    // Serial.print("Task Used Memory: ");
    // Serial.print(taskMem - waterMark);
    // Serial.println(" Bytes");
    // Serial.println("");

  }
}
void setup() {
  Serial.begin(115200);

  int heapSize = ESP.getHeapSize();
  Serial.print("Total Heap Size:  ");
  Serial.print(heapSize);
  Serial.println(" Bytes");

  int heapFree = ESP.getFreeHeap();
  Serial.print("Free Heap Size:  ");
  Serial.print(heapFree);
  Serial.println(" Bytes");
  Serial.println("");


  Serial.println("Create Task ...");
  xTaskCreate(task, "", taskMem, NULL, 1, &taskHandle);

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
  Serial.println(" Bytes");

}

void loop() {

}