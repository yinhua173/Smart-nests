#include "BME680.h"
Adafruit_BME680 bme;
#define SEALEVELPRESSURE_HPA (1013.25)
SemaphoreHandle_t xMutexBME680 = NULL; //创建信号量Handler
TickType_t timeOut = 1000; //用于获取信号量的Timeout 1000 ticks

BME680 bme680;
void bme680Task(void *pvParam) {
  xMutexBME680 = xSemaphoreCreateMutex(); //创建MUTEX信号量
  bme.begin();

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  while (1) {

    if (xSemaphoreTake(xMutexBME680, timeOut) == pdPASS) {

      bme680.temp = bme.temperature;
      bme680.humi = bme.humidity;
      bme680.pres = bme.pressure;
      bme680.gas_res = bme.gas_resistance;
      bme680.alti = bme.readAltitude(SEALEVELPRESSURE_HPA);

      xSemaphoreGive(xMutexBME680); //释放钥匙
    } else {
      //Unable to obtain MUTEX
    }

    vTaskDelay(500);
  }
}

void printTask(void *ptParam) {  //LCD任务主体

  while (1) {

    if (xSemaphoreTake(xMutexBME680, timeOut) == pdPASS) {
    // Tell BME680 to begin measurement.
    unsigned long endTime = bme.beginReading();
    if (endTime == 0) {
      Serial.println(F("Failed to begin reading :("));
      return;
    }
    Serial.print(F("Reading started at "));
    Serial.print(millis());
    Serial.print(F(" and will finish at "));
    Serial.println(endTime);
    if (!bme.endReading()) {
      Serial.println(F("Failed to complete reading :("));
      return;
    } 
    Serial.print(F("Temperature = "));
    Serial.print(bme680.temp);
    Serial.println(F(" *C"));
    Serial.print(F("Pressure = "));
    Serial.print(bme680.pres / 100.0);
    Serial.println(F(" hPa"));
    Serial.print(F("Humidity = "));
    Serial.print(bme680.humi);
    Serial.println(F(" %"));
    Serial.print(F("Gas = "));
    Serial.print(bme680.gas_res / 1000.0);
    Serial.println(F(" KOhms"));
    Serial.print(F("Approx. Altitude = "));
    Serial.print(bme680.alti);
    Serial.println(F(" m"));
        xSemaphoreGive(xMutexBME680); //释放钥匙
      } else {
        //Unable to obtain MUTEX
      }

    vTaskDelay(1000);
  }
}

