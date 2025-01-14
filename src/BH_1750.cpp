#include "BH_1750.h"
TaskHandle_t bh1750Handle = NULL;
/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/

/*
光亮度数据参考：
晚上： 0.001-0.02；
月夜： 0.02-0.3；
多云室内： 5-50；
多云室外： 50-500；
晴天室内： 100-1000；
夏天中午光照下： 大约10*6能量；
阅读书籍时的照明度：50-60；
家庭录像标准照明度：1400
*/
BH1750 lightMeter(0x23);
volatile float lux = 0;
/**************************************************************************/
/*!
    @brief  光照采集
    @returns True if password is correct
*/
/**************************************************************************/
void BH1750Task(void *parameter) {
  //Serial.begin(9600);
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    //Serial.println(F("BH1750 Advanced begin"));
  } else {
    //Serial.println(F("Error initialising BH1750"));
  }
  float lux_date[10]={0};
  while (true) {
    // After setting up the sensor, you should wait a bit before
    // using it, or else you'll get a zero reading
    if (lightMeter.measurementReady()) {
      //lux = lightMeter.readLightLevel();
      lux = GildeAverageValueFilter(lightMeter.readLightLevel(),lux_date,10);
    }
    vTaskDelay(50);
  }
}
