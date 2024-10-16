#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "fingerprint.h"
#include "fingerdata.h"
#include "TTimer.h"
// extern TimerHandle_t xTimeHandle[2];
// TickType_t tick_finger_enroll = xTaskGetTickCount();
extern volatile uint8_t timeout;
extern uint8_t min_one;
extern volatile bool enroll_remove_flag;
extern volatile bool enroll_success_flag;
extern volatile bool enroll_fail_flag;

//SoftwareSerial mySerial(16, 17);//esp32(rt, tx)
#define mySerial Serial2  

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
volatile uint8_t id_static;
uint8_t enroll_inti(){
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  return 0;
}
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available()){
      vTaskDelay(100);
      if(min_one!=timeout)break;
    }
    if(min_one!=timeout){
      Serial.println("Ready too long, please try again");
      enroll_fail_flag = true;
      break;
    }
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintEnroll() {
  uint8_t num = 0;
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    num<50?p = finger.getImage():0;
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      num++>=50?Serial.println("Ready too long, please try again"),p=100:0;
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
    if(p==100)break;
    vTaskDelay(100);
  }
  if(p==100)return 100;
  // OK success!

  p = finger.image2Tz(1);
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

  Serial.println("Remove finger");
  enroll_remove_flag = true;
  delay(2000);
  num = 0;
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    num<50?p = finger.getImage():0;
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      num++>=50?Serial.println("Ready too long, please try again"),p=100:0;
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
    if(p==100)break;
    vTaskDelay(100);
  }
  if(p==100)return 100;
  // OK success!

  p = finger.image2Tz(2);
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
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");enroll_success_flag = true;vector_to_add(id);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return 1;
}
uint8_t enroll_run(){
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  //插入一个计时器清零
  timeout=min_one;

  id = array_out_first();//ID由串口输入
  id_static = id;
  if (id == 0) {// ID #0 not allowed, try again!
     return 0;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  getFingerprintEnroll()==100?enroll_fail_flag=true:0;
  
  if(enroll_fail_flag==false){
    vector_to_add(id);
    laser_task();
    xTaskCreatePinnedToCore(shell_sort_task, "shell_sort_task", 1024*2, NULL, 2, NULL, 0);
    id = array_out_first();
  }

  return 0;
}