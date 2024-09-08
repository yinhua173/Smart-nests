#include "wifi_connect.h"
const char* ssid = "FF";//WiFi名称
const char* password = "528528528";//WiFi密码
volatile uint8_t unlink_state = 0;//WIFI连接超时次数
void ruanjianchongqi(){
  Serial.println("软件重启");
  ESP.restart();
}
void WiFi_connect(){//连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    unlink_state++;
    if(unlink_state>=10){
      ruanjianchongqi();
      unlink_state = 0;
    }
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void wifiTask(void *parameter){
  WiFi_connect();
  while (true){
    if(WiFi.status() != WL_CONNECTED){
        WiFi_connect();
    }else{
      vTaskDelay(100);
    }
  }
}

