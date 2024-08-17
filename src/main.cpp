#include <Arduino.h>
/*
   程序：  消息队列
   公众号：孤独的二进制

   API：
    QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength,
                             UBaseType_t uxItemSize );

    BaseType_t xQueueSend(
                            QueueHandle_t xQueue,
                            const void * pvItemToQueue,
                            TickType_t xTicksToWait
                         );
    BaseType_t xQueueReceive(
                               QueueHandle_t xQueue,
                               void *pvBuffer,
                               TickType_t xTicksToWait
                            );

*/
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 20, 4);

QueueHandle_t queueMsg = xQueueCreate(8, sizeof(char[20]));

//返回随机的文字
String randomMsg() {
  String myStrings[] = {
    "Nice to meet you",
    "Where are U from?",
    "What do you do?",
    "What do U like?",
    "What is UR num?",
    "Do U have FB?",
    "Thanks so much.",
    "I am Chinese.",
    "I do not KNOW.",
    "Thank you.",
    "That helps.",
    "I Love U",
    "Do U miss me?",
    "Be careful.",
    "Don't worry.",
    "Good idea.",
    "He's right.",
    "I ate already.",
    "More than that.",
    "Nothing else.",
    "See you later.",
    "Take it outside.",
  };
  return myStrings[random(0, 22)];
}

void userA(void *ptParam) {
  char msg[20];
  String userID = "A: ";

  while (1) {
    (userID  + randomMsg()).toCharArray(msg, 20);

    TickType_t timeOut = portMAX_DELAY;
    //TickType_t timeOut = 10;
    if (xQueueSend(queueMsg, &msg, timeOut) != pdPASS)  {
      Serial.print(userID);
      Serial.println("Queue is full.");
    };

    vTaskDelay(2000);
  }
}

void userB(void *ptParam) {
  char msg[20];
  String userID = "B: ";

  while (1) {
    (userID  + randomMsg()).toCharArray(msg, 20);

    //portMAX_DELAY - 无限Block
    TickType_t timeOut = portMAX_DELAY;
    //TickType_t timeOut = 10;
    if (xQueueSend(queueMsg, &msg, timeOut) != pdPASS)  {
      Serial.print(userID);
      Serial.println("Queue is full.");
    };

    vTaskDelay(2000);
  }
}

void userC(void *ptParam) {
  char msg[20];
  String userID = "C: ";

  while (1) {
    (userID  + randomMsg()).toCharArray(msg, 20);

    //portMAX_DELAY - 无限Block
    TickType_t timeOut = portMAX_DELAY;
    //TickType_t timeOut = 10;
    if (xQueueSend(queueMsg, &msg, timeOut) != pdPASS)  {
      Serial.print(userID);
      Serial.println("Queue is full.");
    };

    vTaskDelay(2000);
  }
}




void lcdTask(void *ptParam) {  //LCD任务主体

  //lcd.init();
  //lcd.backlight();

  char line0[20] = {' '};
  char line1[20] = {' '};
  char line2[20] = {' '};
  char line3[20] = {' '};
  char * lines[] = { line0, line1, line2, line3 };

  while (1) {
    //文字向上滚动
    strcpy(line0, line1);
    strcpy(line1, line2);
    strcpy(line2, line3);

    //TickType_t timeOut = portMAX_DELAY;
    TickType_t timeOut = 10;
    if (xQueueReceive(queueMsg, lines[3], timeOut) == pdPASS) {
      //显示所有的4行文字
      for (int i = 3; i >= 0; i--) {
        //lcd.setCursor(0, i);
        //lcd.print("                    "); //clear this line
        //lcd.setCursor(0, i);
        //lcd.print(lines[i]);
      }
    }  else {
      Serial.println("Message Queue is Empty");
    };



    vTaskDelay(10);
  }
}

void setup()
{
  Serial.begin(115200);

  xTaskCreate(userA, "User A", 1024 * 8, NULL, 1, NULL);
  xTaskCreate(userB, "User B", 1024 * 8, NULL, 1, NULL);
  xTaskCreate(userC, "User C", 1024 * 8, NULL, 1, NULL);

  xTaskCreate(lcdTask, "lcd", 1024 * 8, NULL, 1, NULL);
}


void loop() {}