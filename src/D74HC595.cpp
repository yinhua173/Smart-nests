#include "D74HC595.h"
volatile bool fire_wt=false;
volatile bool smoke_wt=false;
volatile bool pir_wt=false;
volatile bool rain_wt=false;
volatile bool touch_wt=false;
volatile bool door_wt=false;
void D74HC595_init(){
  pinMode(SHCP,OUTPUT);
  pinMode(STCP,OUTPUT);
  pinMode(DS,OUTPUT);
  D74HC595(0x00);
}

void D74HC595(byte data){
  for(int i=0;i<8;i++){
    digitalWrite(DS,bitRead(data,i));
    digitalWrite(SHCP, HIGH);
    digitalWrite(SHCP, LOW);
  }
  digitalWrite(STCP, HIGH);
  digitalWrite(STCP, LOW);
}
volatile byte data=0x00;//0000 0000
// volatile byte data_1=0x01;//0000 0001
// volatile byte data_2=0x02;//0000 0010
// volatile byte data_3=0x04;//0000 0100
// volatile byte data_4=0x08;//0000 1000
// volatile byte data_5=0x10;//0001 0000
// volatile byte data_6=0x20;//0010 0000
// volatile byte data_7=0x40;//0100 0000
// volatile byte data_8=0x80;//1000 0000
/*
0000 0000|0000 0001=0000 0001//通过或写入
0000 0001^0000 0001=0000 0000//通过异或写出
*/
void D74HC595_loop(){
  if(fire.status||smoke.status||pir.status||rain.status||touch.status||door_flag
      ||fire_wt||smoke_wt||pir_wt||rain_wt||touch_wt||door_wt){
    if(fire.status&&!fire_wt){//火灾--水泵
      data=data|0x01;
      fire_wt=true;
    }else if(!fire.status&&fire_wt){
      data=data^0x01;
    }
    if(smoke.status&&!smoke_wt){//烟雾--开窗--风扇
      data=data|0x02;
      smoke_wt=true;
    }else if(!smoke.status&&smoke_wt){
      data=data^0x02;
    }
    if(pir.status&&!pir_wt){//人体感应--开灯
      data=data|0x04;
      pir_wt=true;
    }else if(!pir.status&&pir_wt){
      data=data^0x04;
    }
    if(rain.status&&!rain_wt){//雨水--关窗
      data=data|0x08;
      rain_wt=true;
    }else if(!rain.status&&rain_wt){
      data=data^0x08;
    }
    if(touch.status&&!touch_wt){//触摸--窗帘
      data=data|0x10;
      touch_wt=true;
    }else if(!touch.status&&touch_wt){
      data=data^0x10;
    }
    if(door_flag&&!door_wt){//门磁
      data=data|0x20;
      door_wt=true;
    }else if(!door_flag&&door_wt){
      data=data^0x20;
    }
    D74HC595(data);
  }
}
void D74HC595Task(void *pvParam){
  D74HC595_init();
  while(1){
    D74HC595_loop();
    vTaskDelay(100);
  }
}