#include "display_menu.h"
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,SCL,SDA,U8X8_PIN_NONE);
const char *menu[MENU_SIZE] = {"天气查看", "门口控制", "窗户控制", "报警查看"};
const char *tianqi[MENU_SIZE] = {"天气预报", "室内状况", "室内温度历史数据", "室内湿度历史数据"};
const char *future_weather[MENU_SIZE] = {"今天|", "明天|", "后天|", "大后天|"};
const char *menkou[MENU_SIZE] = {"门口状态", "开门", "关门", "开门历史数据"};//门口现状：//开关//指纹设置//门口历史
const char *chuanghu[MENU_SIZE] = {"窗户状态", "打开", "关闭", "窗户历史数据"};
const char *baojin[MENU_SIZE] = {"报警状态", "火灾报警查看", "烟雾报警查看", "历史数据"};
const char *onoff[2] = {"开", "关"};
// 定义当前选项
volatile unsigned int  order = 0;
volatile unsigned int  order_2 = 0;
volatile unsigned int  yema = 0;
uint8_t on = 0;
volatile uint8_t future_flag = 0;
volatile bool datadata_state = false;
void OLEDTask(void *pvParam){
    Wire1.begin(SDA, SCL);
    // 初始化 OLED 对象
    u8g2.begin();
    // 开启中文字符集支持
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);// 设置字体
    //xTaskCreatePinnedToCore(bme680Task, "wifiTask", 1024 * 4, NULL, 1, NULL, 1);
    while(1){
        menu_key();
        menu_xuan();
        vTaskDelay(10);
    }    
}

void menu_key(){
  if(key1_flag||key2_flag||key3_flag||key4_flag){
    vTaskDelay(10);//消抖
    // if(digitalRead(BUTTON_1)||digitalRead(BUTTON_2)||digitalRead(BUTTON_3)||digitalRead(BUTTON_4)){

    // }else{
    //   key1_flag = false;
    //   key2_flag = false;
    //   key3_flag = false;
    //   key4_flag = false;
    // }

    if(key1_flag){ //上    
      datadata_state?order_2=--order_2%8:0;
      order = (order - 1) % 4;
      key1_flag = !key1_flag;
    }else if (key2_flag){//下    
      datadata_state?order_2=++order_2%8:0;
      order = (order + 1) % 4;
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
              on=0;
              break;
            case 3:
              on=1;;
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
              //yema=32;//
              break;
            case 3:
              //yema=33;//
              break;
            case 4:
              yema=34;//历史数据
              break;
        }
        break;
      case 4:
        switch(order+1){
            case 1:
              //yema=41;//
              break;
            case 2:
              yema=42;//
              break;
            case 3:
              yema=43;//
              break;
            case 4:
              yema=44;//历史数据
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
      }
      order = 0;
      key3_flag = !key3_flag;
    }else if (key4_flag){//返回
    yema = yema/10;
    order = 0;
    order_2 = 0;
    key4_flag = !key4_flag;
    
    datadata_state=false;
    datadata_temp=false;
    datadata_humi=false;
    data_stop=0;
    }
  }
}
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
    case 24:
      display_menu24(order);
      break;
    case 34:
      display_menu34(order);
      break;
    case 42:
      display_menu42(order);
      break;
    case 43:
      display_menu43(order);
      break;
    case 44:
      display_menu44(order);
      break;
    case 111:
      display_menu111(order);
      break;
  }
}
void menu_loop(){
  menu_key();
  
  menu_xuan();
}
void zhiwen_menkong(){
  if (finger_flag){
    // 设置光标位置
    u8g2.setCursor(64, 12);
    // 显示文字
    u8g2.print("门已开");
    finger_flag = !finger_flag;//无法判断关门信号，需要添加
  }
  if (finger_error_flag){
    // 设置光标位置
    u8g2.setCursor(64, 12);
    // 显示文字
    u8g2.print("指纹错误");
    finger_error_flag = !finger_error_flag;
  }
}
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
// void display_Time(){
//   if(!wifi_state){
//     u8g2.drawLine(120, 0, 114,6);//X
//     u8g2.drawLine(114, 0, 120,6);
//   }
//   // 设置光标位置
//   u8g2.setCursor(24, 12);
//   // 显示文字
//   u8g2.print("2021-03-29  12:30:50");
// }
void display_menu0(unsigned int index){//主页
  // 进入第一页
  u8g2.firstPage();//画wifi
  do{
    display_wifi();
    // 设置光标位置
    u8g2.setCursor(0, 12);
    // 显示文字
    u8g2.print("菜单");
    u8g2.printf(" |%.2d-%.2d|%.2d:%.2d:%.2d|",time_now.Mon,time_now.Day,time_now.Hour,time_now.Min,time_now.Second);
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
void display_menu2(unsigned int index){//"门口"
  // 进入第一页
  u8g2.firstPage();
  do{
    // 绘制页面内容
    u8g2.drawUTF8(0, 12, "门口控制");
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i]);
        u8g2.drawStr(5+strlen(menkou[i])*4, (i + 2) * 12 + 2, " <<");
      }
      else
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, menkou[i]);
      }
    }
    u8g2.drawUTF8(115, 26, onoff[on]);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu3(unsigned int index){//"窗户"
  // 进入第一页
  u8g2.firstPage();
    do{
    // 绘制页面内容
    u8g2.drawUTF8(0, 12, "窗户控制");
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i]);
        u8g2.drawStr(5+strlen(chuanghu[i])*4, (i + 2) * 12 + 2, " <<");
      }
      else
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, chuanghu[i]);
      }
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu4(unsigned int index){//"报警"
  // 进入第一页
  u8g2.firstPage();
    do{
    // 绘制页面内容
    u8g2.drawUTF8(0, 12, "报警查看");
    u8g2.drawHLine(0, 14, 128);
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, baojin[i]);
        u8g2.drawStr(5+strlen(baojin[i])*4, (i + 2) * 12 + 2, " <<");
      }
      else
      {
        u8g2.drawUTF8(5, (i + 2) * 12 + 2, baojin[i]);
      }
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
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
void display_menu34(unsigned int index){//开窗历史数据
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "开窗历史数据");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "时间");
    u8g2.setCursor(0, 38);
    u8g2.printf("%d点开窗", order);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu42(unsigned int index){//火灾报警查看
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "火灾报警查看");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "时间");
    u8g2.setCursor(0, 38);
    u8g2.printf("%d点报警", order);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu43(unsigned int index){//烟雾报警查看
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "烟雾报警查看");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "时间");
    u8g2.setCursor(0, 38);
    u8g2.printf("%d点报警", order);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu44(unsigned int index){//历史数据
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "历史数据");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "时间");
    u8g2.setCursor(0, 38);
    u8g2.printf("%d点报警", order);
    u8g2.drawUTF8(26, 26, "时间");
    u8g2.setCursor(26, 38);
    u8g2.printf("%d点报警", order);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
