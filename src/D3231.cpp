#include "D3231.h"


DS3231 rtc;

void D3231_setup(){
  Wire.begin();
  while (rtc.begin() != DS3232_OK){
    Serial.println("could not connect, check wires etc");
    vTaskDelay(200);
  }
  // while(!get_time_flag){
  //   Serial.println("Waiting for time to be set");
  // vTaskDelay(1000);
  // }
  vTaskDelay(1000);
  if(get_time_flag){
    get_time_flag = false;
    rtc.setSeconds(time_now.Second);
    rtc.setMinutes(time_now.Min);
    rtc.setHours(time_now.Hour);
    rtc.setWeekDay(time_now.Week);
    rtc.setDay(time_now.Day);
    rtc.setMonth(time_now.Mon);
    rtc.setYear(time_now.Year%2000);
    rtc.write();
  }
}
void new_get_time(){
  if(get_time_flag){
    get_time_flag = false;
    rtc.setSeconds(time_now.Second);
    rtc.setMinutes(time_now.Min);
    rtc.setHours(time_now.Hour);
    rtc.setWeekDay(time_now.Week);
    rtc.setDay(time_now.Day);
    rtc.setMonth(time_now.Mon);
    rtc.setYear(time_now.Year%2000);
    rtc.write();
  }
}
//volatile uint8_t hours_flag = false;
//extern TaskHandle_t Handlehttptask;
void D3231_loop(){
  new_get_time();
  rtc.read();
  //int Taskno=0;
  //if(hours_flag< rtc.minutes()){
    //hours_flag = rtc.minutes()+5;
    //Serial.println("---******--------------******---");
    //Taskno=xTaskCreate(httpTask, "httpTask", 1024*100, NULL, 3, &Handlehttptask);//创建http任务
    //Serial.printf("\n-----------------%d--------------------\n",Taskno);
  //}
  // Serial.print(rtc.lastRead());
  // Serial.print("\t\t");
  // Serial.print(rtc.year());
  // Serial.print('-');
  // Serial.print(rtc.month());
  // Serial.print('-');
  // Serial.print(rtc.day());
  // Serial.print(' ');
  // Serial.print(rtc.hours());
  // Serial.print(':');
  // Serial.print(rtc.minutes());
  // Serial.print(':');
  // Serial.print(rtc.seconds());
  // Serial.print('\n');
  // Serial.print(rtc.weekDay());
  // Serial.println('\n');
}
void D3231Task(void *parameter){
  D3231_setup();
  while (1){
    D3231_loop();
    vTaskDelay(1000);
  }
}
void read_setup()
{
  // Serial.begin(115200);
  // Serial.println();
  // Serial.println(__FILE__);
  // Serial.print("DS3232_LIB_VERSION: ");
  // Serial.println(DS3232_LIB_VERSION);
  // Serial.println();

  Wire.begin();
  while (rtc.begin() != DS3232_OK){
    Serial.println("could not connect, check wires etc");
    vTaskDelay(200);
  }
}


void read_loop()
{
  rtc.read();

  Serial.print(rtc.lastRead());
  Serial.print("\t\t");
  Serial.print(rtc.year());
  Serial.print('-');
  Serial.print(rtc.month());
  Serial.print('-');
  Serial.print(rtc.day());
  Serial.print(' ');
  Serial.print(rtc.hours());
  Serial.print(':');
  Serial.print(rtc.minutes());
  Serial.print(':');
  Serial.print(rtc.seconds());
  Serial.print('\n');
  Serial.print(rtc.weekDay());
  Serial.println('\n');

  delay(2000);
}

void write_setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("DS3232_LIB_VERSION: ");
  Serial.println(DS3232_LIB_VERSION);
  Serial.println();

  Wire.begin();
  if (rtc.begin() != DS3232_OK)
  {
    Serial.println("could not connect, check wires etc");
    // while (1);
  }

  //  parse compiler __DATE__ and __TIME__ ?

  //  adjust to your needs.
  // rtc.setSeconds(00);
  // rtc.setMinutes(59);
  // rtc.setHours(12);
  // rtc.setWeekDay(4);   //  4 = Thursday
  // rtc.setDay(5);
  // rtc.setMonth(12);
  // rtc.setYear(24);
  // rtc.write();
  rtc.setSeconds(time_now.Second);
  rtc.setMinutes(time_now.Min);
  rtc.setHours(time_now.Hour);
  rtc.setWeekDay(time_now.Week);   //  4 = Thursday
  rtc.setDay(time_now.Day);
  rtc.setMonth(time_now.Mon);
  rtc.setYear(time_now.Year%2000);
  rtc.write();
  // time_now.Year = atoi(p);
  // time_now.Mon = atoi(p + 5);
  // time_now.Day = atoi(p + 8);
  // time_now.Hour = atoi(p + 11);
  // time_now.Min = atoi(p + 14);
  // time_now.Second = atoi(p + 17);
  // time_now.Week= atoi(weekn);
}


void write_loop()
{
  rtc.read();

  Serial.print(rtc.lastRead());
  Serial.print("\t");
  printDate(Serial);
  Serial.print(" ");
  printTime(Serial);
  Serial.println();

  delay(random(20000));
}


void printDate(Stream &str)
{
  char buffer[16];
  sprintf(buffer, "%04d-%02d-%02d",
        2000 + rtc.year(), rtc.month(), rtc.day());
  str.print(buffer);
}


void printTime(Stream &str)
{
  char buffer[16];
  sprintf(buffer, "%02d:%02d:%02d",
        rtc.hours(), rtc.minutes(), rtc.seconds());
  str.print(buffer);
}
VL53L0X TOF200;
volatile bool TOF200Flag = false;
volatile int TOF200Distance = 0;
void TOF200_setup()
{
  // Serial.begin(115200);
  Wire.begin();

  TOF200.setTimeout(500);
  while(!TOF200.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    vTaskDelay(1000);
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  TOF200.startContinuous();
}

void TOF200_loop()
{
  TOF200Distance=TOF200.readRangeContinuousMillimeters();
  // Serial.print(TOF200Distance);
  if (TOF200.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  if(TOF200Distance<50){
    TOF200Flag = false;
    Serial.print("距离近,窗帘关闭");
  }else{
    TOF200Flag = true;
  }
  //Serial.println();
}
void TOF200Task(void *parameter){
  TOF200_setup();
  while (1){
    TOF200_loop();
    vTaskDelay(1000);
  }
}
