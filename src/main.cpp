#include <Arduino.h>
/*
  程序：  Counting Semaphore//计数信号量//可以用在OLED显示缓存
  公众号：孤独的二进制

  Counting Semphore时间上是一个内存中的整数，从0 - MAX
  Give +1
  Take -1
*/
SemaphoreHandle_t xSemaPhone = NULL;

String consumerA = "JD";
String consumerB = "TMALL";
String consumerC = "PDD";


void producer(void *paParam) { //制造者 give
  while (1) {

    for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);
    xSemaphoreGive(xSemaPhone);
    Serial.println("...... 手机再放出一台,");
  }
}


void consumer(void *pvParam) { //消费者 take
  String website = *(String *)pvParam;

  while (1) {
    if (xSemaphoreTake(xSemaPhone, portMAX_DELAY) == pdTRUE ) {

      for (int i = 0; i < random(200, 400); i++) vTaskDelay(10);
      Serial.print(website);
      Serial.println("抢到并销售一台: ");

    }
  }
}


void setup() {
  Serial.begin(115200);
  xSemaPhone = xSemaphoreCreateCounting(3, 0);

  xTaskCreate(consumer, "consumer a", 1024 * 6, (void *)&consumerA, 1, NULL);
  xTaskCreate(consumer, "consumer b", 1024 * 6, (void *)&consumerB, 1, NULL);
  xTaskCreate(consumer, "consumer c", 1024 * 6, (void *)&consumerC, 1, NULL);

  xTaskCreate(producer, "producer", 1024 * 6, NULL, 1, NULL);

}
void loop() {
}

