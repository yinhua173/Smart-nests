#include <Arduino.h>
#include <Wire.h>

typedef struct{
  void init(int A,int B){
    pinMode(A,INPUT);
    pinMode(B,OUTPUT);
  }
  void run(int A,int B){
    if(!digitalRead(A)){
      digitalWrite(B,HIGH);
      vTaskDelay(100);
    }else{
      digitalWrite(B,LOW);
      vTaskDelay(100);
    }
  }
  volatile bool nin = 0;
}begin;
begin smoke;
begin fire;
begin rain;
begin pir;
begin touch;
void lightTask(void *parameter) {
  smoke.init(2,3);
  while (true) {
    smoke.run(2,3);
  }
}
void setup() {
  xTaskCreatePinnedToCore(lightTask, "lightTask", 1024 * 8, NULL, 1, NULL, 1);
}

void loop() {
}