#include <Arduino.h>
#include "smoke.h"
#include "fingermain.h"

#define SHCP 25
#define STCP 26
#define DS 13

extern begin smoke;
extern begin fire;
extern begin rain;
extern begin pir;
extern begin touch;
extern begin door;

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