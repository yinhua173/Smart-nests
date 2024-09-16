#include "button.h"

KEY button1;
KEY button2;
KEY button3;
KEY button4;
KEY button5;



void buttonTask(void *pvParam){
    pinMode(4,INPUT);
    volatile float value=0;
    while(1){
        value=analogRead(4);
        //Serial.println(value);
        button1.read(2700,2900,value);
        button2.read(2250,2450,value);
        button3.read(1950,2150,value);
        button4.read(1550,1750,value);
        button5.read(1200,1400,value);
        // Serial.print(button1.flag);
        // Serial.print(button2.flag);
        // Serial.print(button3.flag);
        // Serial.print(button4.flag);
        // Serial.println(button5.flag);
        // Serial.println("");
        vTaskDelay(10);
    }
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