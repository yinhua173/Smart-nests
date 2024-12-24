#include "BME680.h"
#include "BH1750.h"
#include "smoke.h"
TaskHandle_t bme680Handle = NULL; 
extern float lux;

Adafruit_BME680 bme;
#define SEALEVELPRESSURE_HPA (1013.25)
SemaphoreHandle_t xMutexBME680 = NULL; //创建信号量Handler
TickType_t timeOut = 1000; //用于获取信号量的Timeout 1000 ticks

BME680 bme680;
extern beginn smoke;
extern beginn fire;
extern beginn rain;
extern beginn pir;
extern beginn touch;
void bme680Task(void *pvParam) {
  xMutexBME680 = xSemaphoreCreateMutex(); //创建MUTEX信号量
  Wire.begin();
  bme.begin();

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  uint8_t times=0;
  while (1) {

    if (xSemaphoreTake(xMutexBME680, timeOut) == pdPASS) {
      bme680.temp=GildeAverageValueFilter(bme.temperature,bme680.temp_date,8);
      bme680.humi=GildeAverageValueFilter(bme.humidity,bme680.humi_date,8);
      bme680.pres=GildeAverageValueFilter(bme.pressure / 100.0,bme680.pres_date,8);
      bme680.alti=GildeAverageValueFilter(bme.readAltitude(SEALEVELPRESSURE_HPA),bme680.alti_date,8);
      // bme680.temp_date[date] = bme.temperature;//温度
      // bme680.humi_date[date] = bme.humidity;//湿度
      // bme680.pres_date[date] = bme.pressure / 100.0;//气压
      // //bme680.gas_res = bme.gas_resistance;//气阻
      // bme680.alti_date[date] = bme.readAltitude(SEALEVELPRESSURE_HPA);//海拔
      xSemaphoreGive(xMutexBME680); //释放钥匙
    } else {
      //Unable to obtain MUTEX
    }
    if(times++%20==0){
      Serial.printf("$bme680%3d680%3d$\r\n",(int)(bme680.temp*10),(int)(bme680.humi*10));
      times=1;
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
    Serial.println(F("\n-----------------------------------"));
    Serial.print(F("Temperature = "));
    Serial.print(bme680.temp);
    Serial.println(F(" *C"));
    Serial.print(F("Pressure = "));
    Serial.print(bme680.pres);
    Serial.println(F(" hPa"));
    Serial.print(F("Humidity = "));
    Serial.print(bme680.humi);
    Serial.println(F(" %"));
    // Serial.print(F("Gas = "));
    // Serial.print(bme680.gas_res / 1000.0);
    // Serial.println(F(" KOhms"));
    Serial.print(F("Approx. Altitude = "));
    Serial.print(bme680.alti);
    Serial.println(F(" m"));
    Serial.println(F("-----------------------------------"));
    Serial.print(F("lux = "));
    Serial.print(lux);
    Serial.println(F(" lux"));
    Serial.println(F("-----------------------------------"));
    Serial.println(smoke.status);
    Serial.println(fire.status);
    Serial.println(rain.status);//亮了为0
    Serial.println(pir.status);
    Serial.println(touch.status);
    Serial.println("*******************************");
        xSemaphoreGive(xMutexBME680); //释放钥匙
      } else {
        //Unable to obtain MUTEX
      }

    vTaskDelay(1000);
  }
}

//滑动平均滤波
float GildeAverageValueFilter(float NewValue,float *Data,unsigned short int windows){
  float max,min;
  float sum;
  unsigned char i;
  Data[0]=NewValue;
  max=Data[0];
  min=Data[0];
  sum=Data[0];
  for(i=windows-1;i!=0;i--)	//循环四次，从后往前  
  {
    if(Data[i]>max) max=Data[i];
    else if(Data[i]<min) min=Data[i];
    sum+=Data[i];
    Data[i]=Data[i-1];	//数据右移
  }
  i=windows-2;
  sum=sum-max-min;
  sum=sum/i;
  return(sum);
}
