#include <Arduino.h>
#include <Wire.h>
#include "BME680.h"

#ifndef SMOKE_HH
#define SMOKE_HH

//lux = GildeAverageValueFilter(lightMeter.readLightLevel(),lux_date,10);

typedef struct{
  volatile bool status = false;
  volatile float value = 0;
  float date[30]={0};
  float value_date[30]={0};
  void init(int A){
    pinMode(A,INPUT);
  }
  void run(int A){
    //GildeAverageValueFilter(digitalRead(A),date,5);
    if(GildeAverageValueFilter(digitalRead(A),date,30)<0.5){
      status = false;
    }else{
      status = true;
    }
  }
  void Arun(float value){//模拟量
    if(GildeAverageValueFilter(value,value_date,30)<3000){
      status = false;
    }else{
      status = true;
    }
  }
}beginn;

void smokeTask(void *parameter);
void fireTask(void *parameter);
void rainTask(void *parameter);
void pirTask(void *parameter);
void touchTask(void *parameter);
void smokeprintTask(void *parameter);
void winTask(void *parameter);
void doorTask(void *parameter);

#endif