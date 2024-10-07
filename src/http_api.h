#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "wifi_connect.h"


#ifndef HTTP_API_H
#define HTTP_API_H

void http_api();
void httpTask(void *parameter);
#endif
