#include <Arduino.h>
#include <Wire.h>

#ifndef SMOKE_HH
#define SMOKE_HH

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

void smokeTask(void *parameter);
void fireTask(void *parameter);
void rainTask(void *parameter);
void pirTask(void *parameter);
void touchTask(void *parameter);

#endif