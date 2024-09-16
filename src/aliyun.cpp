#include "aliyun.h"

/* 设备的三元组信息*/
#define PRODUCT_KEY "a1ta4iTL0RN"
#define DEVICE_NAME "ESPM"
#define DEVICE_SECRET "34464ee765d5f38ed98ae4af52d19e97"
#define REGION_ID "cn-shanghai"

/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT 1883
#define MQTT_USRNAME DEVICE_NAME "&" PRODUCT_KEY

#define CLIENT_ID "a1ta4iTL0RN.ESPM|securemode=2,signmethod=hmacsha256,timestamp=1726388666641|"
#define MQTT_PASSWD "461c4f1ea6785e203dce30cbeddb27d322aad901816a4b5ce343bc9598de2587"

// 宏定义订阅主题
#define ALINK_BODY_FORMAT "{\"id\":\"ESPM\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"

const char *TOPIC = "/sys/a1ta4iTL0RN/ESPM/thing/service/property/set";
unsigned long lastMs = 0;

WiFiClient espClient;
PubSubClient client(espClient);

float soil_data=72.1;
float tep=25.6;

// mqtt连接
void mqttCheckConnect(){
    while (!client.connected()){
        Serial.println("Connecting to MQTT Server ...");
        if (client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD)){
            Serial.println("MQTT Connected!");
        }
        else{
            Serial.print("MQTT Connect err:");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

// 上传温湿度、二氧化碳浓度
void mqttIntervalPost(){
    char param[32];
    char jsonBuf[128];

    // upload humidity
    //soil_data = dht.readHumidity();//读取湿度
    sprintf(param, "{\"humidity\":%2f}", soil_data);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean b = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(b){
        Serial.println("publish Humidity success");
    }else{
        Serial.println("publish Humidity fail");
    }

    // Upload temperature
    //tep = dht.readTemperature();//读取温度
    sprintf(param, "{\"temperature\":%2f}", tep);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean c = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(c){
        Serial.println("publish Temperature success");
    }else{
        Serial.println("publish Temperature fail");
    }
}

// 回调函数
void callback(char *topic, byte *payload, unsigned int length){
    Serial.print("Message arrived [");
    Serial.print(topic); // 打印主题信息
    Serial.print("] ");
    for (int i = 0; i < length; i++){
        Serial.print((char)payload[i]); // 打印主题内容
    }
    Serial.println();

    DynamicJsonDocument doc(1024);                 // 创建了一个名为 doc 的动态 JSON 文档
    deserializeJson(doc, String((char *)payload)); // 从一个名为 payload 的数据中解析 JSON 数据并将其填充到 doc 中

    // DynamicJsonDocument params = doc["params"];

    if(doc["params"].containsKey("LED1")){
        Serial.println("GOT LED1 CMD");
        //digitalWrite(LED1, doc["params"]["LED1"]);//受控端读取与写入
    }
}


void aliyunTask(void *parameter){
    client.setServer(MQTT_SERVER, MQTT_PORT); /* 连接MQTT服务器 */
    client.setCallback(callback);
    while (1){
        if (WiFi.status() != WL_CONNECTED){
            WiFi_connect();
        }
        if (millis() - lastMs >= 5000){
            lastMs = millis();
            mqttCheckConnect();
            /* 上报 */
            mqttIntervalPost();
        }
        client.loop();
        vTaskDelay(2000);
    }
}