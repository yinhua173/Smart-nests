#include <Arduino.h>
/*
   WOKWI模拟器没有看门狗，请大家放到真实ESP32上
   程序： 看门狗 WatchDog
   公众号：孤独的二进制

   说明：看门狗是针对Task任务的
        Arduion-ESP32 默认在 Core 0 的 IDLE 任务开启了看门狗 时间为 5000 ticks = 5秒
        Core 0 和 Core 1 都运行了 FreeRTOS的IDLE任务，优先级为 0
                         IDLE任务是用于清理被删除任务的内存
        Core 1 loopBack任务就是Arduino的 setup 和 loop 优先级为 1

   待解决：程序中如何看所有开启看门狗的任务

*/
#include "esp_task_wdt.h" //Used by esp_task_wdt_reset()

#define CORE_ZERO 0
#define CORE_ONE 1

//feedTheDogInAllTasks()
//通过寄存器给所有任务的狗喂时
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
void feedTheDogInAllTasks() { //通过寄存器给所有任务的狗喂时
  // feed dog 0
  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG0.wdt_feed = 1;                     // feed dog
  TIMERG0.wdt_wprotect = 0;                 // write protect
  // feed dog 1
  TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG1.wdt_feed = 1;                     // feed dog
  TIMERG1.wdt_wprotect = 0;                 // write protect
}


// 永远不休息的task
void taskWithoutBreak(void * ptParam) {
  while (1) {
    //vTaskDelay(1); //让Task进入Block状态，退让资源
    //vTaskSuspend(NULL); //让Task进入Suspend状态，退让资源
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //手动关闭CPU上的TWDT - 慎重操作
  //  disableCore0WDT();
  //  disableCore1WDT();

  //给本任务添加看门狗（NULL代表本任务）
  //这个命令需要放在demo2()的前面，否则连运行的机会都没有
  esp_task_wdt_add(NULL);

  //移除本任务的看门狗（NULL代表本任务）
  //esp_task_wdt_delete(NULL)

  demo1(); //核心0
  //demo2(); //核心1

}

void loop() {
  // esp_task_wdt_reset(); //给自己任务的狗喂时间
  // feedTheDogInAllTasks(); //通过寄存器给所有任务的狗喂时间

  Serial.println("loopBack Task - Priority 1");
  vTaskDelay(100);
}

/*
   Demo 1:
   Core 0 上有两个任务分别是: IDLE(0),  taskWithoutBreak(1)
   因为taskWithoutBreak优先级高，而且永远不会退让出资源，所以IDLE(0)永远不会运行
   Core 0 IDLE(0) 在Arduino-ESP32上默认开启了 Watch Dog， 时间是5秒
   因为IDLE(0)无法运行，所以不能喂狗，导致5秒后重启，内容如下

  E (10156) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
  E (10156) task_wdt:  - IDLE (CPU 0)
  E (10156) task_wdt: Tasks currently running:
  E (10156) task_wdt: CPU 0: TASK-NO-BREAK
  E (10156) task_wdt: CPU 1: loopTask
  E (10156) task_wdt: Aborting.
*/
void demo1() {
  xTaskCreatePinnedToCore(taskWithoutBreak, "TASK-NO-BREAK", 1024, NULL, 1, NULL, CORE_ZERO);
}

/*
   Demo 2:
   Core 1 上有两个任务分别是: IDLE(0),  loopBack(1), taskWithoutBreak(2)
   因为taskWithoutBreak优先级高，而且永远不会退让出资源，所以IDLE(0)和loopBack(1)永远不会运行
   Core 1 IDLE(1) 在Arduino-ESP32上默认没有开启了 Watch Dog， 所以不会自动重启
*/
void demo2() {
  xTaskCreatePinnedToCore(taskWithoutBreak, "TASK-NO-BREAK", 1024, NULL, 2, NULL, CORE_ONE);
}






