#include "display_menu.h"
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,SCL,SDA,U8X8_PIN_NONE);
char *menu[MENU_SIZE] = {"天气查看", "门口控制", "窗户控制", "报警查看"};
char *tianqi[MENU_SIZE] = {"天气预报", "室内状况", "室内温度历史数据", "室内湿度历史数据"};
char *menkou[MENU_SIZE] = {"门口状态", "开门", "关门", "开门历史数据"};
char *chuanghu[MENU_SIZE] = {"窗户状态", "打开", "关闭", "窗户历史数据"};
char *baojin[MENU_SIZE] = {"报警状态", "火灾报警查看", "烟雾报警查看", "历史数据"};
char *onoff[2] = {"开", "关"};
// 定义当前选项
unsigned int  order = 0;
unsigned int  yema = 0;
uint8_t on = 0;
void OLEDTask(void *pvParam){
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
  if(button1.flag) //上
  {
    order = (order - 1) % 4;
  }
  if (button2.flag)//下
  {
    order = (order + 1) % 4;
  }
  if (button3.flag)//确认
  {
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
    }
    order = 0;
  }
  if (button4.flag)//返回
  {
    yema = yema/10;
    order = 0;
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
  }
}
void menu_loop(){
  menu_key();

  menu_xuan();
}

void display_menu0(unsigned int index){//主页
  // 进入第一页
  u8g2.firstPage();
  do
  {
    // 设置光标位置
    u8g2.setCursor(0, 12);
    // 显示文字
    u8g2.print("菜单");
    // 绘制页面内容
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
void display_menu11(unsigned int index){//"天气预报"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "天气预报");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "明天");
    u8g2.setCursor(0, 38);
    u8g2.printf("温度: %d", order);
    u8g2.setCursor(42, 38);
    u8g2.printf("天气: %d", order);
    u8g2.setCursor(84, 38);
    u8g2.printf("天气: %d", order);
    u8g2.drawUTF8(0, 50, "后天");
    u8g2.setCursor(0, 62);
    u8g2.printf("温度: %d", order);
    u8g2.setCursor(42, 62);
    u8g2.printf("天气: %d", order);
    u8g2.setCursor(84, 62);
    u8g2.printf("天气: %d", order);
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu12(unsigned int index){//"室内状态"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "室内状态");
    u8g2.drawHLine(0, 14, 128);
    u8g2.setCursor(0, 27);
    u8g2.printf("温度: %.1f ℃", bme680.temp);
    u8g2.setCursor(0, 40);
    u8g2.printf("湿度: %d ", bme680.humi);
    u8g2.print("%");
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu13(unsigned int index){//"室内温度历史"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "时间");
    u8g2.drawUTF8(0, 30, "温度");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "时间");
    u8g2.setCursor(0, 38);
    u8g2.printf("%d点", order);
    u8g2.drawUTF8(26, 26, "温度");
    u8g2.setCursor(26, 38);
    u8g2.printf("%d℃", order);
    for (int i = 0; i < MENU_SIZE; i++)
    {
      if (i == index)
      {
        // 设置光标位置
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(menu[i]);
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(menu[i]);
        u8g2.print(" <<");
      }
      else
      {
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(menu[i]);
        u8g2.setCursor(0, (i + 2) * 12 + 2);
        u8g2.print(menu[i]);
      }
    }
  } while (u8g2.nextPage()); // 进入下一页，如果还有下一页则返回 True.
}
void display_menu14(unsigned int index){//"室内湿度历史"
  // 进入第一页
  u8g2.firstPage();
    do{
      // 绘制页面内容
    u8g2.drawUTF8(0, 12, "室内湿度历史数据");
    u8g2.drawHLine(0, 14, 128);
    u8g2.drawUTF8(0, 26, "时间");
    u8g2.setCursor(0, 38);
    u8g2.printf("%d点", order);
    u8g2.drawUTF8(26, 26, "湿度");
    u8g2.setCursor(26, 38);
    u8g2.printf("%d %", order);
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
