#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#define mySerial Serial2  

Adafruit_Fingerprint finger1 = Adafruit_Fingerprint(&mySerial);

uint8_t Fingerprint_inti( ){
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger1.begin(57600);
  delay(5);
  if (finger1.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger1.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger1.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger1.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger1.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger1.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger1.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger1.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger1.baud_rate);

  finger1.getTemplateCount();

  if (finger1.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger1.templateCount); Serial.println(" templates");
  }
  return 0;
}
uint8_t getFingerprintID() {
  uint8_t p = finger1.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger1.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger1.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger1.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger1.confidence);

  return finger1.fingerID;
}
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger1.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger1.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger1.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger1.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger1.confidence);
  return finger1.fingerID;
}