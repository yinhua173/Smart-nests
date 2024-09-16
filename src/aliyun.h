#include <Arduino.h>
#include "wifi_connect.h"
#include "PubSubClient.h"   ////A client library for MQTT messaging.
#include <ArduinoJson.h>

#ifndef ALIYUN_H
#define ALIYUN_H

void aliyunTask(void *parameter);
#endif