


#ifndef DATADATA_H
#define DATADATA_H

#include <Arduino.h>
#include "aliyun.h"
#include "BME680.h"
#include "D3231.h"
#include "DS3232.h"
#include "smoke.h"


extern DS3231 rtc;

void delay_test_task(void *parameter);
void datadata_task(void *parameter);

#endif
