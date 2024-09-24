#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

//SoftwareSerial mySerial(16, 17);//esp32(rt, tx)
#define mySerial Serial2  

Adafruit_Fingerprint finger2 = Adafruit_Fingerprint(&mySerial);
uint8_t delete_inti(){
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nDelete Finger");

  // set the data rate for the sensor serial port
  finger2.begin(57600);

  if (finger2.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  return 0;
}

uint8_t readnumber1(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
uint8_t deleteFingerprint(uint8_t id);
uint8_t delete_run(){
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  uint8_t id = readnumber1();
  if (id == 0) {// ID #0 not allowed, try again!
     return 0;
  }

  Serial.print("Deleting ID #");
  Serial.println(id);

  deleteFingerprint(id);
  return 0;
}
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger2.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }

  return p;
}
