#include <Arduino.h>
#include "http_api.h"
#include <VL53L0X.h>
#include "DS3232.h"
extern Times time_now;
#ifndef D3231_HH
#define D3231_HH

extern volatile bool get_time_flag;

void read_setup();
void read_loop();
void write_setup();
void write_loop();
void printDate(Stream &str);
void printTime(Stream &str);
void D3231Task(void *parameter);
void TOF200_setup();
void TOF200_loop();
void TOF200Task(void *parameter);

#endif