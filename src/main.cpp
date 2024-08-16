#include <Arduino.h>

void task1(void *pt){
  // Task code here
  pinMode(13, OUTPUT);
  while(1){
    digitalWrite(13, !digitalRead(13));
    vTaskDelay(1000);//任务延时1s
  }
}
void task2(void *pt){
  // Task code here
  pinMode(12, OUTPUT);
  while(1){
    digitalWrite(12, !digitalRead(12));
    vTaskDelay(500);//任务延时0.5s
  }
}
void setup() {
  // Initialize the LED pin as an output:
  xTaskCreate(task1, "Task1", 1000, NULL, 1, NULL);
  xTaskCreate(task2, "Task2", 1000, NULL, 1, NULL);
  vTaskStartScheduler();

}

void loop() {

}
