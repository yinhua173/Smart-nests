#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#ifndef BME680_HH
#define BME680_HH


typedef struct {
  float temp;// 温度
  float humi;// 湿度
  float pres;// 压力
  float gas_res;// 气体电阻
  float alti;// 海拔
} BME680;

void bme680Task(void *pvParam);

void printTask(void *ptParam);



#endif