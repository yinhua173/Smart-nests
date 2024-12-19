#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
//#include "datadata.h"

#ifndef BME680_HH
#define BME680_HH


typedef struct {
  float temp;// 温度
  float humi;// 湿度
  float pres;// 压力
//  float gas_res;// 气体电阻
  float alti;// 海拔
  float temp_date[8]={0};// 温度组
  float humi_date[8]= {0};// 湿度组
  float pres_date[8]= {0};// 压力组
  float alti_date[8]= {0};// 海拔组
} BME680;

void bme680Task(void *pvParam);

 void printTask(void *ptParam);
float GildeAverageValueFilter(float NewValue,float *Data,unsigned short int windows);


#endif