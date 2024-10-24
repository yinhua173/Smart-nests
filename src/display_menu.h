
#ifndef DISPLAY_MENU_H
#define DISPLAY_MENU_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "button.h"
#include "BME680.h"
#include "BH1750.h"
#include "smoke.h"
#include "fingermain.h"
#include "wifi_connect.h"
#include "http_api.h"
#include "datadata.h"
#include "fingerdata.h"
#include <Adafruit_Fingerprint.h>

#define SDA 23
#define SCL 18

extern bool key1_flag;
extern bool key2_flag;
extern bool key3_flag;
extern bool key4_flag;
extern bool key5_flag;
extern volatile bool door_flag;
extern volatile bool finger_error_flag;
extern volatile bool wifi_state;

extern volatile bool enroll_flag;
extern volatile bool delete_flag;
extern Adafruit_Fingerprint finger1;

extern volatile bool enroll_remove_flag;
extern volatile bool enroll_success_flag;
extern volatile bool enroll_fail_flag;
extern volatile bool enroll_again_flag;

extern volatile bool delete_success_flag;
extern volatile bool delete_fail_flag;

extern std::vector<int> myVector;

extern volatile bool datadata_temp;
extern volatile bool datadata_humi;
extern volatile uint8_t data_stop;

extern volatile float datadata[];
extern volatile int timedd[];
extern volatile uint8_t time_size;

extern String city;

extern BME680 bme680;

extern begin smoke;
extern begin fire;
extern begin rain;
extern begin pir;
extern begin touch;
extern begin door;

extern tian_qi Tianqi;
extern Times time_now;
extern future Future0;
extern future Future1;
extern future Future2;
extern future Future3;
extern future Future4;

#define MENU_SIZE 4
void OLEDTask(void *pvParam);
void menu_key();
void menu_xuan();
//void menu_begin();
void menu_loop();
void display_menu0(unsigned int index);
void display_menu1(unsigned int index);
void display_menu2(unsigned int index);
void display_menu3(unsigned int index);
void display_menu4(unsigned int index);
void display_menu11(unsigned int index);
void display_menu111(unsigned int index);
void display_menu12(unsigned int index);
void display_menu13(unsigned int index);
void display_menu14(unsigned int index);
void display_menu23(unsigned int index);
void display_menu231(unsigned int index, uint8_t index2);
void display_menu232(unsigned int index);
void display_menu24(unsigned int index);
void display_menu34(unsigned int index);
void display_menu42(unsigned int index);
void display_menu43(unsigned int index);
void display_menu44(unsigned int index);

void delete_data();

#endif