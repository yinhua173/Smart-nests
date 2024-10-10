#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "wifi_connect.h"


#ifndef HTTP_API_H
#define HTTP_API_H
typedef struct times{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Second;
}Times;
typedef struct{
    unsigned int temp;
    unsigned int humi;
    unsigned int wid;
    String info;
    String direct;
    String power;
    int aqi;
}tian_qi;

typedef struct{
    String date;
    uint16_t date_year;
    uint8_t date_mon;
    uint8_t date_day;
    void date_math(){
        char p[11];
        int i, j = date.length();
        for (i = 0; i < j; i++){
            p[i] = date[i];
        }
        p[i] = '\0';
        date_year = atoi(p);
        date_mon = atoi(p + 5);
        date_day = atoi(p + 8);
    }
    String temp;
    int8_t temp_max;
    int8_t temp_min;
    void temp_math(){
        char p[9];
        int i, j = temp.length();
        for (i = 0; i < j; i++){
            p[i] = temp[i];
        }
        p[i] = '\0';
        temp_min = atoi(p);
        temp_max = atoi(p + 3);
    }
    String weather;
    unsigned int day;
    unsigned int night;
    char const *direct;
}future;



void http_api();
void httpTask(void *parameter);
#endif
