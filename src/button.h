#include <Arduino.h>

#define LED1 27
#define LED2 26
#define LED3 25
#define LED4 33
#define LED5 32

#ifndef BUTTON_HH
#define BUTTON_HH

typedef struct {
  //volatile float value=0;
  volatile bool flag=false;
}KEY;

void buttonTask(void *pvParam);
void ledTask(void *pvParam);
#endif