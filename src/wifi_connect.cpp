#include "wifi_connect.h"
const char* ssid0 = "FF";//WiFi名称
const char* password0 = "528528528";//WiFi密码
const char* ssid = "FFF";//WiFi名称
const char* password = "fjyzdshp";//WiFi密码
const char* ssid1 = "meng-lpde";//WiFi名称
const char* password1 = "fjyhpmds";//WiFi密码
volatile uint16_t unlink_state = 0;//WIFI连接超时次数
volatile bool wifi_state = false;//WIFI连接状态
void ruanjianchongqi(){
  Serial.println("软件重启");
  ESP.restart();
}
void WiFi_connect(){//连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    unlink_state++;
    wifi_state = false;
    if(unlink_state>=50000){
      ruanjianchongqi();
      unlink_state = 0;
    }else if(unlink_state%6==0||unlink_state%10==2||unlink_state%10==4){
      WiFi.disconnect();
      WiFi.reconnect();
    }else if(unlink_state%6==1){
      WiFi.disconnect();//断开连接
      WiFi.begin(ssid, password);//重新连接
    }else if(unlink_state%6==3){
      WiFi.disconnect();//断开连接
      WiFi.begin(ssid0, password0);//重新连接
    }else if(unlink_state%6==5){
      WiFi.disconnect();//断开连接
      WiFi.begin(ssid1, password1);//重新连接
    }
    // Serial.println("Connecting to WiFi...");
    vTaskDelay(1000);
  }
  wifi_state = true;
  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void wifiTask(void *parameter){
  WiFi_connect();
  while (true){
    if(WiFi.status() != WL_CONNECTED){
        WiFi_connect();
        vTaskDelay(5);
    }else{
      vTaskDelay(1000);
    }
  }
}

