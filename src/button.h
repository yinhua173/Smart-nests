#include <Arduino.h>

#ifndef BUTTON_HH
#define BUTTON_HH

typedef struct {
  //volatile float value=0;
  volatile bool flag=false;
  void read(int num1,int num2,float value){
    //this->value=digitalRead(2);
    if(value>num1 && num2>value){
        flag=true;
    }else{
        flag=false;
    }
  }
}KEY;

void buttonTask(void *pvParam);
void ledTask(void *pvParam);
#endif