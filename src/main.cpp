#include <Arduino.h>
/*
  程序： Software Timer
  公众号：孤独的二进制
  API：
    xTimerCreate //创建时间
    xTimerStart //时间开始
    到时间后，会运行callback函数
*/

TimerHandle_t lockHandle, checkHandle;

void carKey(void *ptParam) {
  byte lockPin = 23;
  pinMode(lockPin, INPUT_PULLUP);

  for (;;) {
    if (digitalRead(lockPin) == LOW) {
      //timeout 3000 ticks
      //xTimerStart 只是开启时间而已，而不是创造时间对象
      //所以如果多次按按钮的话，不会有多个时间对象生成
      //多次按按钮相当于每次对timer进行reset xTimerReset()
      if (xTimerStart(lockHandle, 3000) == pdPASS) {
        Serial.println("About to lock the car");
      } else {
        Serial.println("Unable to lock the car");
      };
      vTaskDelay(100); //very rude Button Debounce
    }

  }
}

void lockCarCallback(TimerHandle_t xTimer) {
  Serial.println("Timer CallBack: Car is Locked");
}

void checkCallback(TimerHandle_t xTimer) {
  // ------- 很复杂的检测汽车Sensors的方法，时间不定 ---------
  Serial.print(xTaskGetTickCount());Serial.println("  -  All Sensors are working."); vTaskDelay(random(10, 90));
}

void setup() {
  Serial.begin(115200);
  xTaskCreate(carKey,
              "Check If Owner Press Lock Button",
              1024 * 1,
              NULL,
              1,
              NULL);

  lockHandle = xTimerCreate("Lock Car",
                            2000,
                            pdFALSE,
                            (void *)0,
                            lockCarCallback);

  checkHandle = xTimerCreate("Sensors Check",
                             100,
                             pdTRUE,
                             (void *)1,
                             checkCallback);

  //必须要在 portMAX_DELAY 内开启 timer start
  //portMAX_DELAY is listed as value for waiting indefinitely
  //实际上0xFFFFFFFF 2^32-1  49天 7周
  //在此期间，此task进入Block状态
  xTimerStart(checkHandle, portMAX_DELAY);
}

void loop() {
}
