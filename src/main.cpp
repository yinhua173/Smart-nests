#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>

/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/
BH1750 lightMeter(0x23);
volatile float lux = 0;
void lightTask(void *parameter) {
  Serial.begin(9600);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
  while (true) {
    // After setting up the sensor, you should wait a bit before
    // using it, or else you'll get a zero reading
    if (lightMeter.measurementReady()) {
      lux = lightMeter.readLightLevel();
    }
    vTaskDelay(200);
  }
}
void setup() {
  xTaskCreatePinnedToCore(lightTask, "lightTask", 1024 * 8, NULL, 1, NULL, 1);
}

void loop() {
}