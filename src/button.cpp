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
        vTaskDelay(100);
    }
}
void ledTask(void *pvParam){
    pinMode(27,OUTPUT);
    pinMode(26,OUTPUT);
    pinMode(25,OUTPUT);
    pinMode(33,OUTPUT);
    pinMode(32,OUTPUT);
    digitalWrite(27,HIGH);
    digitalWrite(26,HIGH);
    digitalWrite(25,HIGH);
    digitalWrite(33,HIGH);
    digitalWrite(32,HIGH);
    while(1){
        if(button1.flag){
            digitalWrite(27,LOW);
        }else if(button2.flag){
            digitalWrite(26,LOW);
        }else if(button3.flag){
            digitalWrite(25,LOW);
        }else if(button4.flag){
            digitalWrite(33,LOW);
        }else if(button5.flag){
            digitalWrite(32,LOW);
        }else{
            digitalWrite(27,HIGH);
            digitalWrite(26,HIGH);
            digitalWrite(25,HIGH);
            digitalWrite(33,HIGH);
            digitalWrite(32,HIGH);
        }
        vTaskDelay(100);
    }
}