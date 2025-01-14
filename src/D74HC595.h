#include <Arduino.h>
#include "smoke.h"
#include "fingermain.h"
#include "BH_1750.h"
#include "D3231.h"

#define SHCP 25
#define STCP 26
#define DS 13
extern DS3231 rtc;
extern beginn smoke;
extern beginn fire;
extern beginn rain;
extern beginn pir;
extern beginn touch;
extern beginn door;
extern beginn win;

extern float lux;

extern VL53L0X TOF200;
extern volatile bool TOF200Flag;
extern volatile int TOF200Distance;

extern volatile bool door_flag;//屏幕开锁--网络开锁--指纹开锁
#ifndef D74HC595_h
#define D74HC595_h
void D74HC595_init();
void D74HC595(byte data);
void D74HC595_loop();
void D74HC595Task(void *pvParam);
void motor_run();
void motor_back();
void motor_clear();
void motor_mode();
#endif