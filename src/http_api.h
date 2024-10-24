#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "wifi_connect.h"


#ifndef HTTP_API_H
#define HTTP_API_H
typedef struct times{
	uint16_t Year;
	uint8_t Mon;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Second;
    uint8_t Week;
}Times;
typedef struct{
    unsigned int temp;
    unsigned int humi;
    unsigned int wid;
    const char * info;
    String info_S;
    const char * direct;
    String direct_S;
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
    char const * weather;
    String weather_S;
    unsigned int day;
    unsigned int night;
    char const *direct;
    String direct_S;
}future;



void http_api();
void httpTask(void *parameter);
#endif
