#include <Arduino.h>
/*
  程序：  Binary Semaphore 是一种信号机制 在中断中的用法
         一个任务(生产者)发出信号。另外一个任务(消费者)接受信号
  公众号：孤独的二进制

  语法：
  SemaphoreHandle_t xHandler; 创建Handler
  xHandler = xSemaphoreCreateBinary(); 创建一个二进制信号量 返回NULL，或者handler
  xSemaphoreGive(xHandler); 生产者+1
  xSemaphoreTake(xHanlder, timeout); 消费者-1 返回pdPASS, 或者pdFAIL

  二进制信号量可以想成就是一个整数 0 或者 1
  Give就是+1
  Take就是-1

  Take的时候如果这个整数是0的话，就等待一直到timeout
*/

volatile bool btnPressed = true;
volatile bool btnReleased = false;
volatile int counter = 0;

SemaphoreHandle_t xSemaLED = NULL; //创建信号量Handler
volatile TickType_t btnDeounce = 0; //用于button Debounce

TickType_t timeOut = 1000; //用于获取信号量的Timeout 1000 ticks

void flashLED(void *pvParam) {

  pinMode(23, OUTPUT);
  while (1) {
    if (xSemaphoreTake( xSemaLED, timeOut) == pdTRUE )
    {
      if ((xTaskGetTickCount() - btnDeounce) < 100) { //用于button debounce
        digitalWrite(23, !digitalRead(23));
        vTaskDelay(1000);
      }
    }
  }
}

void readBtn(void *pvParam) {

  pinMode(22, INPUT_PULLUP);

  while (1) {
    if (digitalRead(22) == LOW) {
      xSemaphoreGive(xSemaLED);
    }
  }
}


void IRAM_ATTR ISR() {
  btnDeounce = xTaskGetTickCountFromISR();
  xSemaphoreGiveFromISR(xSemaLED, NULL);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xSemaLED = xSemaphoreCreateBinary(); //创建二进制信号量

  if (xSemaLED == NULL) {
    printf("No Enough Ram, Unable to Create Semaphore.");
  } else {
    xTaskCreate(flashLED,
                "Flash LED",
                1024 * 4,
                NULL,
                1,
                NULL);
    // xTaskCreate(readBtn,
    //             "Read Button",
    //             1024 * 4,
    //             NULL,
    //             1,
    //             NULL);
  }

  pinMode(22, INPUT_PULLUP);
  attachInterrupt(22, ISR, FALLING);
}



void loop() {
}

