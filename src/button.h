#include <Arduino.h>

// #define LED1 26
// #define LED2 25
// #define LED3 14
// #define LED4 13
// #define LED5 5
// #define LED6 2

#define BUTTON_1 14
#define BUTTON_2 15
#define BUTTON_3 32
#define BUTTON_4 33

#ifndef BUTTON_H
#define BUTTON_H
// 定义外部中断函数

void handle_interrupt4();
void handle_interrupt5();
void handle_interrupt6();
void handle_interrupt7();


void buttonTask();
// void ledTask(void *pvParam);
#endif
