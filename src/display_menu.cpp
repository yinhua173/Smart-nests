#include "display_menu.h"
#include <iostream>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,SCL,SDA,U8X8_PIN_NONE);
const char *menu[MENU_SIZE] = {"天气查看", "门口控制", "窗户控制", "报警查看"};
const char *tianqi[MENU_SIZE] = {"天气预报", "室内状况", "室内温度历史数据", "室内湿度历史数据"};
const char *future_weather[MENU_SIZE] = {"今天|", "明天|", "后天|", "大后天|"};
const char *menkou[MENU_SIZE+1] = {"门口现状", "开门", "关门","指纹设置", "开门历史数据"};//门口现状：//开关//指纹设置//门口历史
const char *chuanghu[MENU_SIZE+1] = {"窗户状态", "打开", "关闭","窗户自动模式"};
const char *curtain[MENU_SIZE+1] = {"窗帘状态", "打开", "关闭","窗帘自动模式"};
const char *onoff[2] = {"关闭", "开启"};
const char *zhiwen[MENU_SIZE]={"录入指纹","删除指纹"};
// const char *shuzi[10]={"录入指纹","删除指纹"};
// 定义当前选项
volatile unsigned int  order = 0;
volatile unsigned int  order_2 = 0;
volatile uint8_t order_finger = 0;//范围0-100
volatile unsigned int  yema = 0;
extern uint8_t id;
extern volatile uint8_t id_static;
extern volatile uint8_t delete_num0;
extern volatile uint8_t delete_num1;
volatile uint8_t flash=0;
volatile uint8_t future_flag = 0;
//volatile bool door_flag = 0;
volatile bool datadata_state = false;
extern volatile bool clear_hang_state;
/**
 * OLED显示任务
 * 循环主体
 * menu_key();
 *menu_xuan();
 */
void OLEDTask(void *pvParam){
    Wire1.begin(SDA, SCL);
    // 初始化 OLED 对象
    u8g2.begin();
    // 开启中文字符集支持
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);// 设置字体
    //xTaskCreatePinnedToCore(bme680Task, "wifiTask", 1024 * 4, NULL, 1, NULL, 1);
    // myVector.push_back(1);
    // myVector.push_back(2);
    // myVector.push_back(3);
    // myVector.push_back(4);
    // myVector.push_back(5);
    while(1){
        menu_key();
        menu_xuan();
        vTaskDelay(10);
    }    
}
volatile uint8_t size=0;
//固定数据
void delete_data(){
  size=vector_out_size();
  Serial.printf("size:%d",size);
}
/**
 * 按键控制
 */
void menu_key(){
  if(key1_flag||key2_flag||key3_flag||key4_flag){
    vTaskDelay(10);//消抖

    if(key1_flag){ //上    
      datadata_state?order_2=--order_2%8:0;
      if(++order_finger>200)order_finger=0;
      order = (order - 1) % 4;
      key1_flag = !key1_flag;
    }else if (key2_flag){//下   
      datadata_state?order_2=++order_2%8:0;
      order = (order + 1) % 4;
      if(--order_finger>200)order_finger=0;
      key2_flag = !key2_flag;
    }else if (key3_flag){//确认
      switch (yema){
      
      case 0:
        switch(order+1){
            case 1:
              yema=1;//天气查看
              break;
            case 2:
              yema=2;//门口控制
              break;
            case 3:
              yema=3;//窗户控制
              break;
            case 4:
              yema=4;//报警查看
              break;
        }
        break;
      case 1:
        switch(order+1){
            case 1:
              yema=11;//天气预报
              break;
            case 2:
              yema=12;//室内状况
              break;
            case 3:
              yema=13;//室内温度历史数据
              break;
            case 4:
              yema=14;//室内湿度历史数据
              break;
        }
        break;
      case 2:
        switch(order+1){
            case 1:
              //yema=21;//
              break;
            case 2:
              door_flag = !door_flag;//开关门
              break;
            case 3:
              yema=23;//指纹设置
              enroll_remove_flag=false;
              enroll_success_flag=false;
              enroll_fail_flag=false;
              break;
            case 4:
              yema=24;//历史数据
              break;
        }
        break;
      case 3:
        switch(order+1){
            case 1:
              //yema=31;//
              break;
            case 2:
              //yema=32;//开关
              break;
            case 3:
              //yema=33;//
              break;
            case 4:
              //yema=34;//历史数据
              break;
        }
        break;
      case 4:
        switch(order+1){
            case 1:
              //yema=41;//
              break;
            case 2:
              //yema=42;//开关
              break;
            case 3:
              //yema=43;//
              break;
            case 4:
              //yema=44;//历史数据
              break;
        }
        break;
      case 11:
        switch(order+1){
          case 1:
            future_flag = 1;
            yema = 111;
            break;
          case 2:
            future_flag = 2;
            yema = 111;
            break;
          case 3:
            future_flag = 3;
            yema = 111;
            break;
          case 4:
            future_flag = 4;
            yema = 111;
            break;
        }
        break;
      case 23:
        switch(order+1){
          case 1:
            yema = 231;
            enroll_flag = true;
            break;
          case 2:
            yema = 232;
            delete_data();
            // delete_flag = true;
            break;
          case 3:
            yema = 231;
            enroll_flag = true;
            break;
          case 4:
            yema = 232;
            delete_data();
            // delete_flag = true;
            break;
        }
        break;
      case 231:
        if(enroll_flag==false){
          //id = array_out_first();
          enroll_flag = true;
          enroll_remove_flag=false;
          enroll_success_flag=false;
          enroll_again_flag=false;
          enroll_fail_flag=false;
          clear_hang_state=true;
        }
        break;
      case 232:
        if(delete_flag==false){
          delete_data();
          delete_flag = true;
          delete_success_flag=false;
          delete_fail_flag=false;
          clear_hang_state=true;
        }
          // enroll_remove_flag=false;
          // enroll_success_flag=false;
          // enroll_fail_flag=false;
          // clear_hang_state=true;
        
        break;
      }
      order = 0;
      key3_flag = !key3_flag;
    }else if (key4_flag){//返回
    yema = yema/10;
    order = 0;
    order_2 = 0;
    key4_flag = !key4_flag;
    
    enroll_flag = false;
    enroll_success_flag=false;
    enroll_fail_flag=false;
    
    delete_flag=false;
    delete_success_flag=false;
    delete_fail_flag=false;

    clear_hang_state=true;
    datadata_state=false;
    datadata_temp=false;
    datadata_humi=false;
    data_stop=0;
    }
    if(yema==232){
      if(delete_num1>order_finger){
        delete_num1=order_finger;
        delete_num0--;
      }
      else if(delete_num1<order_finger){
        order_finger==size?order_finger--:delete_num0++;
        delete_num1=order_finger;
      }
    }
  }
}
/**
 * 选择页面显示
 */
void menu_xuan(){
  switch(yema){
    case 0:
      display_menu0(order);
      break;
    case 1:
      display_menu1(order);
      break;
    case 2:
      display_menu2(order);
      break;
    case 3:
      display_menu3(order);
      break;
    case 4:
      display_menu4(order);
      break;
    case 11:
      display_menu11(order);
      break;
    case 12:
      display_menu12(order);
      break;
    case 13:
      display_menu13(order);
      break;
    case 14:
      display_menu14(order);
      break;
    case 23:
      display_menu23(order);
      break;
    case 24:
      display_menu24(order);
      break;
    case 111:
      display_menu111(order);
      break;
    case 231:
      display_menu231(order, id);
      break;
    case 232:
      display_menu232(order);
      break;
  }
}
void menu_loop(){
  menu_key();
  
  menu_xuan();
}
/**
 * 开门显示
 */
void zhiwen_menkong(){
  if (door_flag){
    // 设置光标位置
    u8g2.setCursor(64, 12);
    // 显示文字
    u8g2.print("门已开");
    door_flag = !door_flag;//无法判断关门信号，需要添加
  }
  if (finger_error_flag){
    // 设置光标位置
    u8g2.setCursor(64, 12);
    // 显示文字
    u8g2.print("指纹错误");//FINGERPRINT_NOMATCH指纹不匹配
    finger_error_flag = !finger_error_flag;
  }
}
/**
 * wifi状态显示
 */
void display_wifi(){
  if(!wifi_state){
    u8g2.drawLine(120, 0, 114,6);//X
    u8g2.drawLine(114, 0, 120,6);
  }
  u8g2.drawCircle(127, 12, 1, U8G2_DRAW_UPPER_LEFT);//画出WiFi形状
  u8g2.drawCircle(127, 12, 3, U8G2_DRAW_UPPER_LEFT);
  u8g2.drawCircle(127, 12, 5, U8G2_DRAW_UPPER_LEFT);
  u8g2.drawCircle(127, 12, 7, U8G2_DRAW_UPPER_LEFT);
}
/*
*主页，wifi，时间
*"天气查看", "门口控制", "窗户控制", "报警查看"
*/
void display_menu0(unsigned int index){//主页
  // 进入第一页
  //烟雾过大，火灾发生，报警显示
  u8g2.firstPage();//画wifi
  do{
    display_wifi();
    // 设置光标位置
    u8g2.setCursor(0, 12);
    // 显示文字
    u8g2.print("菜单");
    u8g2.printf(" |%.2d-%.2d|%.2d:%.2d:%.2d|",rtc.month(),rtc.day(),rtc.hours(),rtc.minutes(),rtc.seconds());
    zhiwen_menkong();
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      {
        // 设置光标位置
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(menu[i]);
        u8g2.print(" <<");
      }
      else
      {
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(menu[i]);
      }
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 天气
 * "天气预报", "室内状况", "室内温度历史数据", "室内湿度历史数据"
 */
void display_menu1(unsigned int index){//"天气"
  // 进入第一页
  u8g2.firstPage();
  do
  {
    // 绘制页面内容
    u8g2.drawUTF8(0, 12, "天气查看");
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, tianqi[i]);
        u8g2.drawStr(5+strlen(tianqi[i])*4, (i + 2) * 12 + 2, " <<");
      }
      else
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, tianqi[i]);
      }
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 门口
 * "门口现状", "开门", "关门","指纹设置", "开门历史数据"
 */
void display_menu2(unsigned int index){//"门口"
  // 进入第一页
  u8g2.firstPage();
  do{
    // 绘制页面内容
    u8g2.drawUTF8(0, 12, "门口控制");
    u8g2.drawHLine(0, 14, 128);
    zhiwen_menkong();
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      { 
        switch(i+1){
          case 1:
            u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i]);
            u8g2.drawStr(5+strlen(menkou[i])*4, (i + 2) * 12 + 2, " <<");
            break;
          case 2:
            door.status?u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i+1]):u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i]);
            u8g2.drawStr(5+strlen(menkou[i])*4, (i + 2) * 12 + 2, " <<");
            break;
          case 3:
            u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i+1]);
            u8g2.drawStr(5+strlen(menkou[i+1])*4, (i + 2) * 12 + 2, " <<");
            break;
          case 4:
            u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i+1]);
            u8g2.drawStr(5+strlen(menkou[i+1])*4, (i + 2) * 12 + 2, " <<");
            break;
        }
      }
      else
      {
        switch(i+1){
          case 1:
            u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i]);
            break;
          case 2:
            door.status?u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i+1]):u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i]);
            break;
          case 3:
            u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i+1]);
            break;
          case 4:
            u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i+1]);
            break;
        }
      }
    }
    u8g2.drawUTF8(103, 26, onoff[door.status]);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 窗户
 * "窗户状态", "打开", "关闭", "窗帘设置","窗户自动模式"
 */
void display_menu3(unsigned int index){//"窗户"
  // 进入第一页
  u8g2.firstPage();
  do{
  // 绘制页面内容
  u8g2.drawUTF8(0, 12, "窗户控制");
  u8g2.drawHLine(0, 14, 128);
  zhiwen_menkong();
  for (int i = 0; i < MENU_SIZE; i++)
  {
    if (i == index)
    { 
      switch(i+1){
        case 1:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i]);
          u8g2.drawStr(5+strlen(chuanghu[i])*4, (i + 2) * 12 + 2, " <<");
          break;
        case 2:
          door.status?u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i]):u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i+1]);
          u8g2.drawStr(5+strlen(chuanghu[i])*4, (i + 2) * 12 + 2, " <<");
          break;
        case 3:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i+1]);
          u8g2.drawStr(5+strlen(chuanghu[i+1])*4, (i + 2) * 12 + 2, " <<");
          break;
      }
    }
    else
    {
      switch(i+1){
        case 1:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i]);
          break;
        case 2:
          door.status?u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i]):u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i+1]);
          break;
        case 3:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i+1]);
          break;
      }
    }
  }
  u8g2.drawUTF8(103, 26, onoff[win.status]);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 窗帘
 * "窗帘状态", "打开", "关闭" ,"窗帘自动模式"
 */
void display_menu4(unsigned int index){//"窗帘"
  // 进入第一页
  u8g2.firstPage();
  do{
  // 绘制页面内容
  u8g2.drawUTF8(0, 12, "窗帘控制");
  u8g2.drawHLine(0, 14, 128);
  zhiwen_menkong();
  for (int i = 0; i < MENU_SIZE; i++)
  {
    if (i == index)
    { 
      switch(i+1){
        case 1:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i]);
          u8g2.drawStr(5+strlen(curtain[i])*4, (i + 2) * 12 + 2, " <<");
          break;
        case 2:
          door.status?u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i]):u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i+1]);
          u8g2.drawStr(5+strlen(curtain[i])*4, (i + 2) * 12 + 2, " <<");
          break;
        case 3:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i+1]);
          u8g2.drawStr(5+strlen(curtain[i+1])*4, (i + 2) * 12 + 2, " <<");
          break;
      }
    }
    else
    {
      switch(i+1){
        case 1:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i]);
          break;
        case 2:
          door.status?u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i]):u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i+1]);
          break;
        case 3:
          u8g2.drawUTF8(5, (i + 2) * 12 + 2, curtain[i+1]);
          break;
      }
    }
  }
  u8g2.drawUTF8(103, 26, onoff[TOF200Flag]);
  u8g2.setCursor(103+12*2, 26+12);
  u8g2.printf("%d", TOF200Distance/10);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 天气状况
 * "今天|", "明天|", "后天|", "大后天|"
 */
void display_menu11(unsigned int index){//"天气状况"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "天气概况");
    u8g2.setCursor(63, 12);
    u8g2.printf("%s", city);
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "今天:");
    
    u8g2.drawUTF8(0, 38, "明天:");
    //u8g2.printf("%s", Future1.weather);
    u8g2.drawUTF8(0, 50, "后天:");
    //u8g2.printf("%s", Future2.weather);
    u8g2.drawUTF8(0, 62, "大后天:");
    //u8g2.printf("%s", Future3.weather);
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      {
        // 设置光标位置
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(future_weather[i]);
        switch (i){
          case 0:
            u8g2.printf("%s", Future0.weather_S.c_str());
            break;
          case 1:
            u8g2.printf("%s", Future1.weather_S.c_str());
            break;
          case 2:
            u8g2.printf("%s", Future2.weather_S.c_str());
            break;
          case 3:
            u8g2.printf("%s", Future3.weather_S.c_str());
            break;
        }
        u8g2.print(" <<");
      }
      else
      {
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(future_weather[i]);
        switch (i){
          case 0:
            u8g2.printf("%s", Future0.weather_S.c_str());
            break;
          case 1:
            u8g2.printf("%s", Future1.weather_S.c_str());
            break;
          case 2:
            u8g2.printf("%s", Future2.weather_S.c_str());
            break;
          case 3:
            u8g2.printf("%s", Future3.weather_S.c_str());
            break;
        }
      }
    }
    future_flag=order;
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 天气预报
 * DAY0: "今天", DAY1: "明天", DAY2: "后天", DAY3: "大后天"
 * "温度:", "湿度:", "风向:", "风力:"
 */
void display_menu111(unsigned int index){//"天气预报"
  // 进入第一页
  u8g2.firstPage();
    do{
    // 绘制页面内容
    u8g2.drawUTF8(0, 12, "天气预报");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 38, "温度:");
    u8g2.drawUTF8(0, 50, "风向:");
    switch (future_flag)
    {
      case 1:
        u8g2.drawUTF8(63, 12, "|今天");
        u8g2.drawUTF8(63, 26, "|湿度:");
        u8g2.drawUTF8(63, 50, "|风力:");
        u8g2.drawUTF8(0, 62, "空气质量API:");
        u8g2.setCursor(72, 62);
        u8g2.printf("%d", Tianqi.aqi);
        u8g2.setCursor(0, 26);
        u8g2.printf("%s", Tianqi.info_S.c_str());
        u8g2.setCursor(30, 38);
        u8g2.printf("%d~%d℃,当前%d℃", Future0.temp_min, Future0.temp_max,Tianqi.temp);
        u8g2.setCursor(95, 26);
        u8g2.printf("%d",Tianqi.humi);
        u8g2.print("%");
        u8g2.setCursor(30, 50);
        u8g2.printf("%s", Tianqi.direct_S.c_str());
        u8g2.setCursor(95, 50);
        u8g2.printf("%s", Tianqi.power);
        break;
      case 2:
        //Future1.direct="东风转西南风";
        u8g2.drawUTF8(63, 12, "|明天");
        u8g2.setCursor(0, 26);
        u8g2.printf("%s", Future1.weather_S.c_str());
        u8g2.setCursor(30, 38);
        u8g2.printf("%d~%d℃", Future1.temp_min, Future1.temp_max);
        u8g2.setCursor(30, 50);
        u8g2.printf("%s", Future1.direct_S.c_str());
        break;
      case 3:
        u8g2.drawUTF8(63, 12, "|后天");
        u8g2.setCursor(0, 26);
        u8g2.printf("%s", Future2.weather_S.c_str());
        u8g2.setCursor(30, 38);
        u8g2.printf("%d~%d℃", Future2.temp_min, Future2.temp_max);
        u8g2.setCursor(30, 50);
        u8g2.printf("%s", Future2.direct_S.c_str());
        break;
      case 4:
        u8g2.drawUTF8(63, 12, "|大后天");
        u8g2.setCursor(0, 26);
        u8g2.printf("%s", Future3.weather_S.c_str());
        u8g2.setCursor(30, 38);
        u8g2.printf("%d~%d℃", Future3.temp_min, Future3.temp_max);
        u8g2.setCursor(30, 50);
        u8g2.printf("%s", Future3.direct_S.c_str());
        break;
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 室内状况
 * 温度，湿度，气压，海拔
 */
void display_menu12(unsigned int index){//"室内状态"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "室内状态");
    u8g2.drawHLine(0, 14, 128);
    u8g2.setCursor(0, 26);
    u8g2.printf("温度: %.1f ℃", bme680.temp);
    u8g2.setCursor(0, 38);
    u8g2.printf("湿度: %.1f ", bme680.humi);
    u8g2.print("%");
    u8g2.setCursor(0, 50);
    u8g2.printf("气压: %.1f hPa", bme680.pres);
    u8g2.setCursor(0, 62);
    u8g2.printf("海拔: %.1f m", bme680.alti);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 室内温度历史
 * 时间   温度
 */
void display_menu13(unsigned int index){//"室内温度历史"
  // 进入第一页
  datadata_temp=true;
  datadata_state=true;
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "时间");
    u8g2.drawUTF8(63, 12, "温度");
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < 4; i++)
    {
        // 设置光标位置
        u8g2.setCursor(0, 26+i*12);
        u8g2.printf("%d点", timedd[i+order_2*4]);
        u8g2.setCursor(63, 26+i*12);
        u8g2.printf("%.1f℃", datadata[i+order_2*4]);
        u8g2.setCursor(110, 26+i*12);
        u8g2.print(" >>>");
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 室内湿度历史
 * 时间   湿度
 */
void display_menu14(unsigned int index){//"室内湿度历史"
  // 进入第一页
  u8g2.firstPage();
  datadata_humi=true;
  datadata_state=true;
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "时间");
    u8g2.drawUTF8(63, 12, "湿度");
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < 4; i++)
    {
      // 设置光标位置
      u8g2.setCursor(0, 26+i*12);
      u8g2.printf("%d点", timedd[i+order_2*4]);
      u8g2.setCursor(63, 26+i*12);
      u8g2.printf("%.1f%%", datadata[i+order_2*4]);
      u8g2.setCursor(110, 26+i*12);
      u8g2.print(" >>>");
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 指纹设置  指纹个数
 * 录入指纹，删除指纹
 */
void display_menu23(unsigned int index){//"指纹设置"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "已有指纹个数");//finger1.templateCount
    u8g2.setCursor(72, 12);
    u8g2.printf(" %d", finger1.templateCount);
    u8g2.setCursor(96, 12);
    u8g2.printf(" %d", vector_out_size());
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < 2; i++)
    {
      if (i == (index%2))
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, zhiwen[i]);
        u8g2.drawStr(5+strlen(zhiwen[i])*4, (i + 2) * 12 + 2, " <<");
      }
      else
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, zhiwen[i]);
      }
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 录入指纹
 * 请录入指纹 * 请移开手指 * 请重按手指 * 录入成功指纹
 */
void display_menu231(unsigned int index,uint8_t index2){//"录入指纹"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "录入指纹");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "请录入指纹");
    u8g2.setCursor(72, 26);
    u8g2.printf("%.2d", id);
    u8g2.drawStr(72+12, 26, " <<");
    //清行
    if(clear_hang_state||enroll_flag||enroll_remove_flag||enroll_success_flag||enroll_fail_flag||enroll_again_flag){
      if(clear_hang_state){
        u8g2.drawStr(0, 38, "                    ");
        clear_hang_state=false;
      }
      if(enroll_flag){
        u8g2.drawUTF8(0, 38, "请按手指");
      }
      if(enroll_remove_flag){
        u8g2.drawUTF8(0, 38, "请移开手指");
      }
      if(enroll_again_flag){
        u8g2.drawUTF8(0, 38, "请重按手指");
      }
      if(enroll_success_flag){
        u8g2.drawUTF8(0, 38, "录入成功指纹");
        u8g2.setCursor(84, 38);
        u8g2.printf("%.2d", id_static);
      }
      if(enroll_fail_flag){
        u8g2.drawUTF8(0, 38, "指纹录入失败");
      }
    }
    
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 删除指纹  已有指纹
 * 选择指纹 * 删除成功
 */
void display_menu232(unsigned int index){//"删除指纹"
  
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
      u8g2.drawUTF8(0, 12, "删除指纹");
      u8g2.drawHLine(0, 14, 128);
      u8g2.drawUTF8(0, 26, "已有指纹");//流动显示指纹
      u8g2.drawStr(12*5-6, 26, " [[");
      u8g2.setCursor(72, 26);//获得大小，判断循环范围，输出向量
      //size 5 i=1 delete_num0=3,->i=2,delete_num0=4->i=3,delete_num0=5,size==delete_num0->重新
      //size 2 i=1 size>3?delete_num0=3:delete_num0=size;    size==delete_num0?0:i++,delete_num0++;
      if(size){
        if(delete_num0<size){
          u8g2.setCursor(12*6+1, 24);
          u8g2.printf("%.2d", myVector[delete_num0]);
          if(delete_num0+1<size){
            u8g2.setCursor(12*7+4, 26);
            u8g2.printf("%.2d", myVector[delete_num0+1]);
            if(delete_num0+2<size){
              u8g2.setCursor(12*8+8, 26);
              u8g2.printf("%.2d", myVector[delete_num0+2]);
            }else{
              u8g2.drawStr(12*8+8, 26, "          ");
            }
          }else{
            u8g2.drawStr(12*7+4, 26, "           ");
          }
        }
        
        u8g2.drawUTF8(0, 38, "要删除的指纹");//指着第1位
        //u8g2.drawStr(12*6, 36, " ^");//u8g2_DrawTriangle(&u8g2,20,5, 27,50, 5,32);
        //u8g2.drawTriangle(12*6+8, 32, 12*6, 32, 12*6+6, 24);
        if(++flash>1){
          u8g2.drawTriangle(12*6+9, 32, 12*6, 32, 12*6+6, 24);
          if(flash>2){
            flash=0;
          }
        }else{
          u8g2.drawStr(12*6+1, 36, "                    ");
        }
      }else if(size==0){
        u8g2.drawUTF8(12*7, 26, "暂无");
      }
      u8g2.drawStr(12*9+8, 26, "]]");
      if(clear_hang_state||delete_flag||delete_success_flag||delete_fail_flag){
        if(clear_hang_state){
          u8g2.drawStr(12*6+1, 26, "                    ");
          clear_hang_state=false;
        }
        if(delete_success_flag){
          u8g2.drawUTF8(0, 50, "指纹已删除");
          u8g2.setCursor(84, 50);
          delete_data();
          u8g2.printf("%.2d", id_static);
        }
        if(delete_fail_flag){
          u8g2.drawUTF8(0, 50, "删除失败");
        }
      }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
/**
 * 开门历史数据
 */
void display_menu24(unsigned int index){//开门历史数据
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "开门历史数据");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "时间");
    u8g2.setCursor(0, 38);
    u8g2.printf("%d点开门", order);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}

