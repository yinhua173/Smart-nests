#include <Arduino.h>
#include <Wire.h>

#ifndef SMOKE_HH
#define SMOKE_HH

// typedef struct{
//   volatile bool status = false;
//   volatile float value = 0;
//   void init(int A,int B){
//     pinMode(A,INPUT);
//     pinMode(B,OUTPUT);
//     digitalWrite(B,HIGH);
//   }
//   void run(int A,int B){//数字量
//     if(!digitalRead(A)){
//       status = false;
//       digitalWrite(B,HIGH);
//     }else{
//       status = true;
//       digitalWrite(B,LOW);
//     }
//     vTaskDelay(100);
//   }
//   void Arun(float value,int B){//模拟量
//     if(value<3000){
//       status = false;
//       digitalWrite(B,HIGH);
//     }else{
//       status = true;
//       digitalWrite(B,LOW);
//     }
//     vTaskDelay(100);
//   }
// }begin;

typedef struct{
  volatile bool status = false;
  volatile float value = 0;
  void init(int A){
    pinMode(A,INPUT);
  }
  void run(int A){//数字量
    if(!digitalRead(A)){
      status = false;
    }else{
      status = true;
    }
  }
  void Arun(float value){//模拟量
    if(value<3000){
      status = false;
    }else{
      status = true;
    }
  }
}begin;

void smokeTask(void *parameter);
void fireTask(void *parameter);
void rainTask(void *parameter);
void pirTask(void *parameter);
void touchTask(void *parameter);
void smokeprintTask(void *parameter);
void winTask(void *parameter);
void doorTask(void *parameter);

#endif