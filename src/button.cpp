#include "button.h"

bool key1_flag = false;
bool key2_flag = false;
bool key3_flag = false;
bool key4_flag = false;



// 定义外部中断函数
void handle_interrupt4() {
  key1_flag = true;
}
void handle_interrupt5() {
  key2_flag = true;
}
void handle_interrupt6() {
  key3_flag = true;
}
void handle_interrupt7() {
  key4_flag = true;
}

void buttonTask(){
    // 配置输入按键
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLDOWN);
    pinMode(BUTTON_4, INPUT_PULLDOWN);
    // 配置中断引脚
    attachInterrupt(digitalPinToInterrupt(BUTTON_1), handle_interrupt4, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_2), handle_interrupt5, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_3), handle_interrupt6, RISING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_4), handle_interrupt7, FALLING);
    // while(1){
    //     vTaskDelay(10);
    // }
}



void ledTask(void *pvParam){
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    pinMode(LED3,OUTPUT);
    pinMode(LED4,OUTPUT);
    pinMode(LED5,OUTPUT);
    pinMode(LED6,OUTPUT);
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH);
    digitalWrite(LED4,HIGH);
    digitalWrite(LED5,HIGH);
    digitalWrite(LED6,HIGH);
    // digitalWrite(LED1,LOW);
    // digitalWrite(LED2,LOW);
    // digitalWrite(LED3,LOW);
    // digitalWrite(LED4,LOW);
    // digitalWrite(LED5,LOW);
    // digitalWrite(LED6,LOW);
    vTaskDelete(NULL);
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
