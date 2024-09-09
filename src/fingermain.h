#ifndef FINGERMIAN_HH
#define FINGERMAIN_HH
#include <Arduino.h>

void handle_interrupt1();
void handle_interrupt2();
void handle_interrupt3();
uint8_t fingermain_init();
uint8_t fingermain_run();
void fingerTask(void *parameter);
#endif