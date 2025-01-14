
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
#include "D3231.h"
#include "DS3232.h"

#define SDA 23
#define SCL 18

extern bool key1_flag;//上
extern bool key2_flag;//下
extern bool key3_flag;//确认
extern bool key4_flag;//返回
extern volatile bool door_flag;//门的状态
extern volatile bool finger_error_flag;//指纹模块错误
extern volatile bool wifi_state;//wifi连接状态

extern volatile bool win_flag;
extern volatile bool curtain_flag;
extern volatile bool curtain_oo;
extern volatile bool win_aoti;//oled控制窗户自动模式
extern volatile bool curtain_aoti;//oled控制窗帘自动模式

extern volatile bool enroll_flag;//指纹录入
extern volatile bool delete_flag;//指纹删除
extern Adafruit_Fingerprint finger1;//指纹模块

extern volatile bool enroll_remove_flag;//指纹移开
extern volatile bool enroll_success_flag;//指纹录入成功
extern volatile bool enroll_fail_flag;//指纹录入失败  
extern volatile bool enroll_again_flag;//指纹再次录入

extern volatile bool delete_success_flag;//指纹删除成功
extern volatile bool delete_fail_flag;//指纹删除失败

extern std::vector<int> myVector;//指纹库

extern volatile bool datadata_temp;//温度数据包使用标志
extern volatile bool datadata_humi;//湿度数据包使用标志
extern volatile bool datadata_door;//门口数据包使用标志
extern volatile uint8_t data_stop;//数据包结束标志

extern volatile float datadata[];//数据包
extern volatile int timedd[];//时间数据包
// extern volatile uint8_t time_size;

extern volatile bool TOF200Flag;//窗帘开关状态判断
extern volatile int TOF200Distance;//窗帘开启距离
extern float lux;
extern String city;

extern BME680 bme680;
extern DS3231 rtc;

extern beginn smoke;
extern beginn fire;
extern beginn rain;
extern beginn pir;
extern beginn touch;
extern beginn door;
extern beginn win;

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

void delete_data();

#endif