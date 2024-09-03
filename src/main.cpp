#include <Arduino.h>
#include <Wire.h>
#include "smoke.h"

void setup() {
  xTaskCreatePinnedToCore(smokeTask, "lightTask", 1024 * 8, NULL, 1, NULL, 1);
}

void loop() {
}