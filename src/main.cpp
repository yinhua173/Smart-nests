#include <Arduino.h>
/*
  程序： 任务优先级
  公众号：孤独的二进制

  说明：高优先级的任务就像是救护车，低优先级的任务就是普通车辆
       如同公路上所有普通车辆需要让救护车一样
       高优先级任务永远会比低优先级任务优先执行

  问题：高优先级任务不进入Block或者Suspend状态
       低优先级任务就永远不会被执行
       这就是著名的任务 《吃撑和饿死》 的问题

  API：
    设置优先级
    void vTaskPrioritySet( TaskHandle_t xTask,
                       UBaseType_t uxNewPriority );
    
    获取TashHandle任务优先级
    UBaseType_t uxTaskPriorityGet( TaskHandle_t xTask );

    获取当前任务优先级
    UBaseType_t uxTaskPriorityGet(NULL);

    退让资源，任务调度器会重新评估任务，将资源分配给同等级或者更高等级任务
    注意不会把资源给低等级任务
    tastYIELD() yield()
*/
TaskHandle_t xFirstClassHandle = NULL;

void firstClass(void *ptParam) {
  while (1) {
    Serial.print("头等舱客户 - 等级");
    UBaseType_t uTaskPriority =  //显示本任务当前等级
      uxTaskPriorityGet(NULL);
    Serial.println(uTaskPriority);
    taskYIELD(); //资源退让给同等级或者更高级的任务
  }
}

void ecoClass(void *ptParam) {
  while (1) {
    Serial.print("经济舱客户 - 等级");
    UBaseType_t uTaskPriority = //显示本任务当前等级
      uxTaskPriorityGet(NULL);
    Serial.println(uTaskPriority);
    taskYIELD(); //资源退让给同等级或者更高级的任务
  }
}

void controlPanel(void *ptParam) {
  pinMode(23, INPUT_PULLUP);
  while (1) {
    if (digitalRead(23) == LOW) {
      
      //获取头等舱任务的当前等级
      if (xFirstClassHandle != NULL) {
        UBaseType_t uFirstClassPriority =
          uxTaskPriorityGet(xFirstClassHandle);

        switch (uFirstClassPriority) {
          case 2: //降级
            vTaskPrioritySet(xFirstClassHandle, 1);
            break;
          case 1: //升级
            vTaskPrioritySet(xFirstClassHandle, 2);
            break;
          default:
            break;
        }
      }
      vTaskDelay(120); //粗暴的防止按钮抖动
    }

  }
}

void setup() {

  Serial.begin(115200);

  /*
  三个任务都放在Core1
  任务优先等级：普通舱1级， 头等舱2级， 控制台3级
  */

  xTaskCreatePinnedToCore(ecoClass, "ecoClass", 1024 * 2, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(firstClass, "firstClass", 1024 * 2, NULL, 2, &xFirstClassHandle, 1);
  xTaskCreatePinnedToCore(controlPanel, "controlPanel", 1024 * 2, NULL, 3, NULL, 1);

}

void loop() {
}
