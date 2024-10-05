
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


#define SDA 23
#define SCL 18

extern bool key1_flag;
extern bool key2_flag;
extern bool key3_flag;
extern bool key4_flag;
extern bool key5_flag;
extern volatile bool finger_flag;
extern volatile bool finger_error_flag;
extern volatile bool wifi_state;

extern BME680 bme680;

extern begin smoke;
extern begin fire;
extern begin rain;
extern begin pir;
extern begin touch;
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
void display_menu12(unsigned int index);
void display_menu13(unsigned int index);
void display_menu14(unsigned int index);
void display_menu24(unsigned int index);
void display_menu34(unsigned int index);
void display_menu42(unsigned int index);
void display_menu43(unsigned int index);
void display_menu44(unsigned int index);

#endif