#include "D74HC595.h"
volatile bool fire_wt=false;
volatile bool smoke_wt=false;
volatile bool pir_wt=false;
volatile bool rain_wt=false;
volatile bool touch_wt=false;
volatile bool door_wt=false;
volatile bool win_wt=false;
volatile bool win_flag=false;//oled控制窗户
volatile bool curtain_flag=false;//oled控制窗帘
volatile bool win_aoti=false;//oled控制窗户自动模式
volatile bool curtain_aoti=false;//oled控制窗帘自动模式
volatile bool curtain_yunaoti=false;//阿里云控制窗帘自动模式
volatile bool light_wt=false;
volatile bool light_flag=false;
volatile uint8_t curtain_yun=0;
volatile uint8_t touch_t=0;
uint8_t delay_time=5;
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
/**
 * 0x10作为串口输入标识，0x00作为串口结束标识
 */
/*
门，窗，交给语音模块风扇(pin)
开灯(pin)，窗帘(4pin)，水泵(pin)，交给74HC595
*/
void D74HC595_loop(){
  if(fire.status||smoke.status||pir.status||rain.status||touch.status||door_flag||win_flag||curtain_flag
      ||fire_wt||smoke_wt||pir_wt||rain_wt||touch_wt||door_wt||win_wt
      ||curtain_yun){
    if(fire.status&&!fire_wt){//火灾--水泵
      data=data|0x80;
      fire_wt=true;
    }else if(!fire.status&&fire_wt){
      data=data^0x80;
      fire_wt=false;
    }
    if(smoke.status&&!smoke_wt){//烟雾--开窗--风扇
      smoke_wt=true;
      Serial.write("$fan_on$");
      vTaskDelay(1);
      Serial.write("$win_on$");
      vTaskDelay(1);
    }else if(!smoke.status&&smoke_wt){
      smoke_wt=false;
      Serial.write("$fan_off$");
      vTaskDelay(1);
      Serial.write("$win_off$");
      vTaskDelay(1);
    }
    if(pir.status&&!pir_wt){//人体感应--开灯
      pir_wt=true;
      Serial.write("$light_on$");
      vTaskDelay(1);
    }else if(!pir.status&&pir_wt){
      pir_wt=false;
      Serial.write("$light_off$");
      vTaskDelay(1);
    }
    if(rain.status&&!rain_wt&&!win_aoti){//雨水--关窗
      rain_wt=true;
      Serial.write("$win_off$");
      vTaskDelay(1);
    }else if(!rain.status&&rain_wt&&!win_aoti){
      rain_wt=false;
      Serial.write("$win_on$");
      vTaskDelay(1);
    }
    if(win_flag&&!win_wt&&win_aoti){//手动关窗
      win_wt=true;
      Serial.write("$win_off$");
      vTaskDelay(1);
    }else if(!win_flag&&win_wt&&win_aoti){
      win_wt=false;
      Serial.write("$win_on$");
      vTaskDelay(1);
    }
    if(door_flag&&!door_wt){//门磁
      door_wt=true;
      Serial.write("$door_off$");
      vTaskDelay(1);
    }else if(!door_flag&&door_wt){
      door_wt=false;
      Serial.write("$door_on$");
      vTaskDelay(1);
    }
    if(touch.status){//触摸--窗帘(4pin电机)（pin5~8）
      curtain_aoti=true;
      if(!touch_wt){
        touch_t==0?touch_t=1:touch_t==1?touch_t=2:touch_t=1;
        touch_wt=true;
      }
      motor_mode();
    }else if(!touch.status&&touch_wt){
      motor_clear();
      touch_wt=false;
    }else if(!curtain_aoti){
      if(lux>50&&TOF200Distance<100){//自动--窗帘--光，亮---开---距离0，停止
        motor_run();//电机正转开窗帘
      }else if(lux<50&&TOF200Distance>50){
        motor_back();//电机反转关窗帘
      }else if(TOF200Distance>100||TOF200Distance<50){
        motor_clear();//电机停止
      }
    }else if(curtain_yun>0){
      curtain_aoti=true;
      if((curtain_yun>=TOF200Distance-15&&curtain_yun<=TOF200Distance+15)||TOF200Distance>100||TOF200Distance<50){
        motor_clear();//电机停止
        curtain_yun=0;
      }else if(curtain_yun>TOF200Distance){//手动，云--窗帘--光，亮---开---距离0，停止
        motor_run();//电机正转开窗帘
      }else if(curtain_yun<TOF200Distance){
        motor_back();//电机反转关窗帘
      }
    }
    //D74HC595(data);
  }
  if(light_flag&&!light_wt){//灯泡
    data=data|0x40;
    light_wt=true;
  }else if(!light_flag&&light_wt){
    data=data^0x40;
    light_wt=false;
  }
}
void D74HC595Task(void *pvParam){
  D74HC595_init();
  while(1){
    D74HC595_loop();
    // motor_run();
    vTaskDelay(10);
  }
}
void motor_mode(){
  switch (touch_t){
  case 1:
    motor_run();
    //Serial.println("run");
    break;
  case 2:
    motor_back();
    //Serial.println("back");
    break;
  }
}

void motor_run(){
  // 八拍模式
  data=data|0x04;//0000 0100
  data=data^0x04;//0000 0000
  
  data=data|0x08;//0000 1000
  D74HC595(data);
  delay(delay_time);

  data=data|0x01;//0000 1001
  D74HC595(data);
  delay(delay_time);

  data=data^0x08;//0000 0001
  D74HC595(data);
  delay(delay_time);

  data=data|0x02;//0000 0011
  D74HC595(data);
  delay(delay_time);

  data=data^0x01;//0000 0010
  D74HC595(data);
  delay(delay_time);

  data=data|0x04;//0000 0110
  D74HC595(data);
  delay(delay_time);

  data=data^0x02;//0000 0100
  D74HC595(data);
  delay(delay_time);

  data=data|0x08;//0000 1100
  D74HC595(data);
  delay(delay_time);
}
void motor_back(){
  // 八拍模式
  data=data|0x01;//0000 0001    IN4=1
  data=data^0x01;//0000 0000

  data=data|0x08;//0000 1000
  D74HC595(data);
  delay(delay_time);

  data=data|0x04;//0000 1100
  D74HC595(data);
  delay(delay_time);

  data=data^0x08;//0000 0100
  D74HC595(data);
  delay(delay_time);

  data=data|0x02;//0000 0110
  D74HC595(data);
  delay(delay_time);

  data=data^0x04;//0000 0010
  D74HC595(data);
  delay(delay_time);

  data=data|0x01;//0000 0011
  D74HC595(data);
  delay(delay_time);

  data=data^0x02;//0000 0001
  D74HC595(data);
  delay(delay_time);

  data=data|0x08;//0000 1001
  D74HC595(data);
  delay(delay_time);
}
void motor_clear(){
  data=data|0x0F;//0000 1111
  data=data^0x0F;//0000 0000
  D74HC595(data);
}
