#ifndef FINGERDATA_H
#define FINGERDATA_H
#include <Arduino.h>


void shell_sort_task(void *parameter);
void shell_sort(uint8_t arr[], uint8_t len);
void vector_task(void *parameter);
void vector_to_add(int data);
uint8_t vector_out_size();
void laser_task();
uint8_t array_out_first();
void vector_to_delete(int data);

#endif