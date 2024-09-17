#include "button.h"

KEY button1;
KEY button2;
KEY button3;
KEY button4;

#define BUTTON_1 36
#define BUTTON_2 39
#define BUTTON_3 32
#define BUTTON_4 33


void buttonTask(void *pvParam){
    // 配置输入按键
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);
    pinMode(BUTTON_4, INPUT_PULLUP);
    // 配置中断引脚
    attachInterrupt(digitalPinToInterrupt(BUTTON_1), handle_interrupt1, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_2), handle_interrupt2, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_3), handle_interrupt3, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_4), handle_interrupt4, FALLING);
    pinMode(4,INPUT);
    volatile float value=0;
    while(1){
        vTaskDelay(10);
    }
}

// 定义外部中断函数
void handle_interrupt1() {
  button1.flag = true;
}
void handle_interrupt2() {
  button2.flag = true;
}
void handle_interrupt3() {
  button3.flag = true;
}
void handle_interrupt4() {
  button4.flag = true;
}

void ledTask(void *pvParam){
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    pinMode(LED3,OUTPUT);
    pinMode(LED4,OUTPUT);
    pinMode(LED5,OUTPUT);
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH);
    digitalWrite(LED4,HIGH);
    digitalWrite(LED5,HIGH);
    while(1){
        // if(button1.flag){
        //     digitalWrite(LED1,LOW);
        // }else if(button2.flag){
        //     digitalWrite(LED2,LOW);
        // }else if(button3.flag){
        //     digitalWrite(LED3,LOW);
        // }else if(button4.flag){
        //     digitalWrite(LED4,LOW);
        // }else if(button5.flag){
        //     digitalWrite(LED5,LOW);
        // }else{
        //     digitalWrite(LED1,HIGH);
        //     digitalWrite(LED2,HIGH);
        //     digitalWrite(LED3,HIGH);
        //     digitalWrite(LED4,HIGH);
        //     digitalWrite(LED5,HIGH);
        // }
        // vTaskDelay(100);
    }
}