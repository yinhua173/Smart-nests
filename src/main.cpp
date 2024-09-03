#include <Arduino.h>
#include <Wire.h>
#include "smoke.h"
#include "BH_1750.h"

void setup() {
  xTaskCreatePinnedToCore(smokeTask, "lightTask", 1024 * 8, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(BH1750Task, "BH1750Task", 1024 * 8, NULL, 1, NULL, 1);
}

void loop() {
}
