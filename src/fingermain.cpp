#include <Arduino.h>
#include "fingerprint.h"
#include "enroll.h"
#include "delete.h"

#define BUTTON_14  27//判断指纹按下
#define BUTTON_15  15//进入录入
#define BUTTON_13  13//删除指纹
// 定义可以在外部中断函数中使用的变量
volatile bool fingerprint_flag = false;
volatile bool enroll_flag = false;
volatile bool delete_flag = false;
// 定义外部中断函数
void handle_interrupt1() {
  fingerprint_flag = true;
}
void handle_interrupt2() {
  enroll_flag = true;
}
void handle_interrupt3() {
  delete_flag = true;
}
uint8_t fingermain_init(){
  Fingerprint_inti();
  Serial.begin(115200);
  pinMode(BUTTON_14, INPUT_PULLDOWN);
  pinMode(BUTTON_15, INPUT_PULLUP);
  pinMode(BUTTON_13, INPUT_PULLUP);
  //pinMode(BUTTON_13, INPUT_PULLDOWN);
  // 配置中断引脚
  attachInterrupt(digitalPinToInterrupt(BUTTON_14), handle_interrupt1, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_15), handle_interrupt2, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_13), handle_interrupt3, FALLING);
  return 0;
}

uint8_t fingermain_run(){
  if(fingerprint_flag && !enroll_flag && !delete_flag) {    
    getFingerprintID();
    fingerprint_flag = false;
  }
  if(enroll_flag) {
    //enroll_inti();
    enroll_run();
    enroll_flag = false;
    delete_flag = false;
  }
  if(delete_flag) {
    delete_run();
    delete_flag = false;
    enroll_flag = false;
  }
  return 0;
}
void fingerTask(void *parameter) {//数字信号
  fingermain_init();
  while (1) {
    fingermain_run();
    vTaskDelay(100);
  }
}
