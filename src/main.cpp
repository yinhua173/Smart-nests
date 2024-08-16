#include <Arduino.h>

/*
  程序： 绝对任务延迟
  公众号：孤独的二进制
  API：
    vTaskDelayUntil(&xLastWakeTime, xFrequency)
      最后一次的唤醒时间是指针类型。
      本函数会自动更新xLastWakeTime为最后一次唤醒的时间
      所以无需手动在while循环内对其手动赋值
    xTaskGetTickCount()
      Tick Coun 和 Arduino Millis一样
      uint32_t类型 49天后overflow
*/

void showStockTask(void *ptParam) {
  static float stockPrice = 99.57; //股票价格

  //最后一次唤醒的tick count，第一次使用需要赋值
  //以后此变量会由vTaskDelayUntil自动更新
  TickType_t xLastWakeTime = xTaskGetTickCount();

  const TickType_t xFrequency = 3000; // 间隔 3000 ticks = 3 seconds

  for (;;) {
    //恰恰算算，经过思考，既然我们叫做LastWakeTime，那么 vTaskDelayUntil 应该放在循环的第一句话
    //如果放在循环的最后一句话，应该改为xLastSleepTime 才更加合适
    // 看懂的朋友， 请鼓掌
    // 哦，我无法听到掌声，干脆帮我按住 点赞三秒 对我的视频进行强力推荐吧
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    //验证当前唤醒的时刻tick count
    Serial.println(xTaskGetTickCount());
    //验证xLastWake Time是否被vTaskDelayUntil更新
    //Serial.println(xLastWakeTime);

    // ------- 很复杂的交易股票计算，时间不定 ---------
    stockPrice = stockPrice * (1 + random(1, 20) / 100.0); vTaskDelay(random(500, 2000));

    Serial.print("Stock Price : $");
    Serial.println(stockPrice);

    //使用vTaskDelay试试看会如何
    //vTaskDelay(xFrequency);
  }

}

void setup() {
  Serial.begin(115200);
  xTaskCreate(showStockTask, "Show Stock Price", 1024 * 6, NULL, 1, NULL);
}

void loop() {

}
