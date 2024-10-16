#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "fingerprint.h"
TickType_t tick_finger_delete = xTaskGetTickCount();
extern volatile uint16_t timeout;
extern uint8_t min_one;
extern uint8_t id;
extern volatile uint8_t id_static;
extern volatile bool delete_success_flag;
extern volatile bool delete_fail_flag;
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
    while (! Serial.available()){
      vTaskDelay(100);
      if(min_one!=timeout)break;
    }
    if(min_one!=timeout){
      Serial.println("Ready too long, please try again");
      delete_fail_flag = true;
      break;
    }
    num = Serial.parseInt();
  }
  return num;
}
uint8_t deleteFingerprint(uint8_t id);
uint8_t delete_run(){
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  //插入一个计时器清零
  timeout=min_one;
  // uint8_t id = readnumber1();//串口输入删除
  //id = array_out_first();//ID由屏幕输入
  id_static = id;
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
