#include <Arduino.h>
/*
   程序：  多种消息的队列
   公众号：孤独的二进制
   说明：本实例使用结构体巧妙的通过当个队列传输多个设备多种数据类型
        在接收方，我们通过deviceID来判断数据来源和value的意义

   API：
    QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength,
                             UBaseType_t uxItemSize );

    BaseType_t xQueueSend(
                            QueueHandle_t xQueue,
                            const void * pvItemToQueue,
                            TickType_t xTicksToWait
                         );
    BaseType_t xQueueReceive(
                               QueueHandle_t xQueue,
                               void *pvBuffer,
                               TickType_t xTicksToWait
                            );

*/
//#include "DHTesp.h"
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 20, 4);

#define DHT22_ID 0
#define LDR_ID 1

typedef struct {
  byte deviceID;
  float value1;
  float value2;
} SENSOR;

QueueHandle_t queueSensor = xQueueCreate(8, sizeof(SENSOR));

void dht22(void *ptParam) {

  const byte dhtPin = 32;
  //DHTesp dhtSensor;
  //dhtSensor.setup(dhtPin, DHTesp::DHT22);

  SENSOR dht22Sensor;
  dht22Sensor.deviceID = DHT22_ID;

  while (1 ) {
    //TempAndHumidity  data = dhtSensor.getTempAndHumidity();

    // Serial.println("Temp: " + String(data.temperature, 2) + "°C");
    // Serial.println("Humidity: " + String(data.humidity, 1) + "%");

    //dht22Sensor.value1 = data.temperature;
    //dht22Sensor.value2 = data.humidity;

    // TickType_t timeOut = portMAX_DELAY;
    TickType_t timeOut = 2000;
    if (xQueueSend(queueSensor, &dht22Sensor, timeOut) != pdPASS) {
      Serial.println("DHT22: Queue is full.");
    }

    vTaskDelay(1000);
  }

}

void ldr(void *ptParam) {
  const float GAMMA = 0.7;
  const float RL10 = 50;
  const byte ldrPIN = 27;
  pinMode(ldrPIN, INPUT);

  SENSOR ldrSensor;
  ldrSensor.deviceID = LDR_ID;

  while (1 ) {
    int analogValue = analogRead(ldrPIN);

    float voltage = analogValue / 4095. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

    // Serial.print("LDR Light Sensor lux : ");
    // Serial.println(lux);

    ldrSensor.value1 = lux;
    ldrSensor.value2 = 0.0;

    // TickType_t timeOut = portMAX_DELAY;
    TickType_t timeOut = 2000;
    if (xQueueSend(queueSensor, &ldrSensor, timeOut) != pdPASS) {
      Serial.println("LDR: Queue is full.");
    }

    vTaskDelay(1000);
  }

}

void lcdTask(void *ptParam) {  //LCD任务主体

  //lcd.init();
  //lcd.backlight();

  //lcd.setCursor(0, 0);
  //lcd.print("   LONELY  BINARY  ");

  SENSOR data;
  while (1) {
    //TickType_t timeOut = portMAX_DELAY;
    TickType_t timeOut = 2000;
    if (xQueueReceive(queueSensor, &data, timeOut) == pdPASS) {

      switch (data.deviceID) {
        case DHT22_ID:
          //lcd.setCursor(0, 1);
          //lcd.print("Temp: " + String(data.value1, 2) + "c");
          //lcd.setCursor(0, 2);
          //lcd.print("Humidity: " + String(data.value2, 1) + "%");
          break;
        case LDR_ID:
          //lcd.setCursor(0, 3);
          if (data.value1 > 50) {
            //lcd.print("Bright ");
          } else {
            //lcd.print("Dark ");
          }
          //lcd.setCursor(0, 3);
          //lcd.print(String(data.value1, 2) + " lux");
          break;
        default:
          Serial.println("LCD: Unkown Device");
          break;
      }
    }  else {
      Serial.println("LCD: Message Queue is Empty");
    };

    vTaskDelay(2000);
  }
}

void setup()
{
  Serial.begin(115200);

  xTaskCreate(dht22, "DHT22", 1024 * 4, NULL, 1, NULL);
  xTaskCreate(ldr, "LDR LIGHT", 1024 * 4, NULL, 1, NULL);
  xTaskCreate(lcdTask, "lcd", 1024 * 8, NULL, 1, NULL);

}


void loop() {}