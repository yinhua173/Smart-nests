#include "aliyun.h"

/* 设备的三元组信息*/
#define PRODUCT_KEY "a1sf6CO3czA"
#define DEVICE_NAME "ESPX"
#define DEVICE_SECRET "1c945589620b2a64729c5548dac6965c"
#define REGION_ID "cn-shanghai"

/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT 1883
#define MQTT_USRNAME DEVICE_NAME "&" PRODUCT_KEY

#define CLIENT_ID "a1sf6CO3czA.ESPX|securemode=2,signmethod=hmacsha256,timestamp=1731140304212|"
#define MQTT_PASSWD "fcd8ae0978da529c15aba93d49ccca6c59135a1cbf6df9d4e850d3f3fe980104"

// 宏定义订阅主题
#define ALINK_BODY_FORMAT "{\"id\":\"ESPX\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"

const char *TOPIC = "/sys/a1sf6CO3czA/ESPX/thing/service/property/set";
unsigned long lastMs = 0;

WiFiClient espClient;
PubSubClient client(espClient);


extern BME680 bme680;
extern float lux;
extern beginn smoke;
extern beginn fire;
extern beginn rain;
extern beginn pir;
extern beginn touch;
extern beginn door;
extern beginn win;
extern volatile int TOF200Distance;//窗帘开启距离
extern volatile bool door_flag;//门的状态
extern volatile bool light_flag;//灯状态
extern volatile bool win_flag;//窗户状态
extern volatile bool curtain_flag;//窗帘状态
extern volatile bool win_aoti;//oled控制窗户自动模式
extern volatile bool curtain_aoti;//oled控制窗帘自动模式
extern volatile uint8_t curtain_yun;
float  *humi = &bme680.humi;     // 读取湿度
float  *temp = &bme680.temp;     // 读取温度

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
    sprintf(param, "{\"humidity\":%2f}", *humi);
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
    sprintf(param, "{\"temperature\":%2f}", *temp);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean c = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(c){
        Serial.println("publish Temperature success");
    }else{
        Serial.println("publish Temperature fail");
    }

    // Upload LUX
    //tep = dht.readTemperature();//读取亮度
    sprintf(param, "{\"lux\":%2f}", lux);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean d = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(d){
        Serial.println("publish lux success");
    }else{
        Serial.println("publish lux fail");
    }

    // Upload smoke
    //烟雾
    sprintf(param, "{\"smoke\":%d}", smoke.status);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean smoke = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(smoke){
        Serial.println("publish smoke success");
    }else{
        Serial.println("publish smoke fail");
    }

    // Upload fire
    //火焰
    sprintf(param, "{\"fire\":%d}", fire.status);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean fire = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(fire){
        Serial.println("publish fire success");
    }else{
        Serial.println("publish fire fail");
    }

    // Upload rain
    //雨滴
    sprintf(param, "{\"rain\":%d}", rain.status);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean rain = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(rain){
        Serial.println("publish rain success");
    }else{
        Serial.println("publish rain fail");
    }

    // Upload PIR
    //人体红外
    sprintf(param, "{\"PIR\":%d}", pir.status);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean pir = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(pir){
        Serial.println("publish pir success");
    }else{
        Serial.println("publish pir fail");
    }
}
// 上传门口状态,灯,窗户，窗帘
void mqttIntervalPost_1(){
    char param[32];
    char jsonBuf[128];

    // doorback
    //门口状态
    sprintf(param, "{\"doorback\":%d}", door.status);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean doorback = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(doorback){
        Serial.println("publish doorback success");
    }else{
        Serial.println("publish doorback fail");
    }

    // lightback
    //灯的状态
    sprintf(param, "{\"lightback\":%d}", light_flag);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean lightback = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(lightback){
        Serial.println("publish lightback success");
    }else{
        Serial.println("publish lightback fail");
    }

    // winback
    //窗户状态
    sprintf(param, "{\"winback\":%d}", win.status);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean winback = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(winback){
        Serial.println("publish winback success");
    }else{
        Serial.println("publish winback fail");
    }

    // curtainback
    //窗帘状态
    sprintf(param, "{\"curtainback\":%d}", TOF200Distance);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean curtainback = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(curtainback){
        Serial.println("publish curtainback success");
    }else{
        Serial.println("publish curtainback fail");
    }

    // winMode
    //窗户模式状态
    sprintf(param, "{\"winMode\":%d}", win_aoti);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean winMode = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(winMode){
        Serial.println("publish winMode success");
    }else{
        Serial.println("publish winMode fail");
    }

    // curtainmode
    //窗帘模式状态
    sprintf(param, "{\"curtainmode\":%d}", curtain_aoti);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean curtainmode = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(curtainmode){
        Serial.println("publish curtainmode success");
    }else{
        Serial.println("publish curtainmode fail");
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

    if(doc["params"].containsKey("doorback")){
        Serial.println("GOT doorback CMD");
        door_flag = !door.status;//开关门
        //digitalWrite(LED1, doc["params"]["LED1"]);//受控端读取与写入
    }
    if(doc["params"].containsKey("lightback")){
        Serial.println("GOT lightback CMD");
        light_flag = !light_flag;//开关灯
        //digitalWrite(LED3, doc["params"]["LED3"]);//受控端读取与写入
    }
    if(doc["params"].containsKey("winback")){
        Serial.println("GOT winback CMD");
        win_flag = !win.status;//开关窗
        //digitalWrite(LED2, doc["params"]["LED2"]);//受控端读取与写入
    }
    if(doc["params"].containsKey("curtainback")){
        curtain_yun=doc["params"].containsKey("curtainback");
        Serial.println("GOT curtainback CMD");
        //curtain_flag = !curtain_flag;//开关窗帘
        //digitalWrite(LED3, doc["params"]["LED3"]);//受控端读取与写入
    }

    if(doc["params"].containsKey("winMode")){
        Serial.println("GOT winmodeback CMD");
        win_aoti = !win_aoti;//oled控制窗户自动模式

        //digitalWrite(LED2, doc["params"]["LED2"]);//受控端读取与写入
    }
    if(doc["params"].containsKey("curtainmode")){
        Serial.println("GOT curtainmode CMD");
        curtain_aoti =! curtain_aoti;//oled控制窗帘自动模式
        //digitalWrite(LED3, doc["params"]["LED3"]);//受控端读取与写入
    }
}


void aliyunTask(void *parameter){
    client.setServer(MQTT_SERVER, MQTT_PORT); /* 连接MQTT服务器 */
    client.setCallback(callback);
    bool Post= false;
    while (1){
        if (WiFi.status() != WL_CONNECTED){
            WiFi_connect();
        }
        if (millis() - lastMs >= 5000){
            lastMs = millis();
            mqttCheckConnect();
            if(Post){
                /* 上报 */
                mqttIntervalPost();
                vTaskDelay(10);
            }else{
                mqttIntervalPost_1();
                vTaskDelay(10);
            }
            Post =! Post;
        }
        client.loop();
        vTaskDelay(1000);
    }
}