#include "http_api.h"


// 定义http请求
String url = "http://apis.juhe.cn/simpleWeather/query";
String city = "梧州";
String key = "f7a00d7ed724ea602774412f227b894e";
uint8_t http_api_state = 36;//http_api请求次数
HTTPClient http;// 创建 HTTPClient 对象

// 定义http请求
String url_time = "http://apis.juhe.cn/fapigx/worldtime/query";
String city1 = "北京";
String key1 = "b71e51e2f481ff8e50e23ce592bc31d4";
HTTPClient http1;// 创建 HTTPClient 对象

tian_qi Tianqi;
future Future0;
future Future1;
future Future2;
future Future3;
future Future4;
Times time_now;//定义时间结构体
void wid_swtich(){
  switch(Tianqi.wid){
    case 0://"weather": "晴"
      break;
    case 1://"weather": "多云"
      break;
    case 2://"weather": "阴"
      break;
    case 3://"weather": "阵雨"
      break;
    case 4://"weather": "雷阵雨"
      break;
    case 5://"weather": "雷阵雨伴有冰雹"
      break;
    case 6://"weather": "雨夹雪"
      break;
    case 7://"weather": "小雨"
      break;
    case 8://"weather": "中雨"
      break;
    case 9://"weather": "大雨"
      break;
    case 10://"weather": "暴雨"
      break;
    case 11://"weather": "大暴雨"
      break;
    case 12://"weather": "特大暴雨"
      break;
    case 13://"weather": "阵雪"
      break;
    case 14://"weather": "小雪"
      break;
    case 15://"weather": "中雪"
      break;
    case 16://"weather": "大雪"
      break;
    case 17://"weather": "暴雪"
      break;
    case 18://"weather": "雾"
      break;
    case 19://"weather": "冻雨"
      break;
    case 20://"weather": "沙尘暴"
      break;
    case 21://"weather": "小到中雨"
      break;
    case 22://"weather": "中到大雨"
      break;
    case 23://"weather": "大到暴雨"
      break;
    case 24://"weather": "暴雨到大暴雨"
      break;
    case 25://"weather": "大暴雨到特大暴雨"
      break;
    case 26://"weather": "小到中雪"
      break;
    case 27://"weather": "中到大雪"
      break;
    case 28://"weather": "大到暴雪"
      break;
    case 29://"weather": "浮尘"
      break;
    case 30://"weather": "扬沙" 
      break;
    case 31://"weather": "强沙尘暴"
      break;
    case 53://"weather": "霾"
      break;
  }
}
void http_tianqi(){
  // 发送GET请求
  http.begin(url + "?city=" + city + "&key=" + key);

  int httpCode = http.GET();

  // 获取响应状态码
  Serial.printf("HTTP 状态码: %d", httpCode);

  // 获取响应正文
  String response = http.getString();
  Serial.println("响应数据");
  Serial.println(response);

  http.end();

  // 创建 DynamicJsonDocument 对象
  DynamicJsonDocument doc(1024);

  // 解析 JSON 数据
  deserializeJson(doc, response);

  // 从解析后的 JSON 文档中获取值
  // 当天天气
  Tianqi.temp = doc["result"]["realtime"]["temperature"].as<unsigned int>(); // 温度
  Tianqi.humi = doc["result"]["realtime"]["humidity"].as<unsigned int>();    // 湿度
  Tianqi.wid = doc["result"]["realtime"]["wid"].as<unsigned int>();          // 天气
  Tianqi.info = doc["result"]["realtime"]["info"].as<String>();              // 天气
  Tianqi.direct = doc["result"]["realtime"]["direct"].as<String>();          // 风向
  Tianqi.power = doc["result"]["realtime"]["power"].as<String>();            // 风力
  Tianqi.aqi = doc["result"]["realtime"]["aqi"].as<int>();                   // 空气指数

  Serial.printf("温度: %d\n", Tianqi.temp);
  Serial.printf("天气: %s\n", Tianqi.info);
  Serial.printf("空气指数: %d\n", Tianqi.aqi);
  Serial.printf("-------------------------");
  Serial.printf("-------------------------");
  // // 未来天气0
  // Future0.date = doc["result"]["future"][0]["date"].as<String>();         // 日期
  // Future0.temp = doc["result"]["future"][0]["temperature"].as<String>();  // 温度
  // //Future0.humi = doc["result"]["future"][0]["humidity"].as<String>();     // 湿度
  // Future0.weather = doc["result"]["future"][0]["weather"].as<String>();   // 天气
  // Future0.day = doc["result"]["future"][0]["day"].as<unsigned int>();     // 白天温度
  // Future0.night = doc["result"]["future"][0]["night"].as<unsigned int>(); // 夜间温度
  // Future0.direct = doc["result"]["future"][0]["direct"].as<char const *>();     // 风向

  // Serial.printf("日期: %s\n", Future0.date);
  // Serial.printf("温度: %s\n", Future0.temp);
  // Future0.temp_math();
  // Serial.printf("温度max: %d\n", Future0.temp_max);
  // Serial.printf("温度min: %d\n", Future0.temp_min);

  // // 未来天气1
  // Future1.date = doc["result"]["future"][1]["date"].as<String>();         // 日期
  // Future1.temp = doc["result"]["future"][1]["temperature"].as<String>();  // 温度
  // //Future1.humi = doc["result"]["future"][1]["humidity"].as<String>();     // 湿度
  // Future1.weather = doc["result"]["future"][1]["weather"].as<String>();   // 天气
  // Future1.day = doc["result"]["future"][1]["day"].as<unsigned int>();     // 白天温度
  // Future1.night = doc["result"]["future"][1]["night"].as<unsigned int>(); // 夜间温度
  // Future1.direct = doc["result"]["future"][1]["direct"].as<char const *>();     // 风向
  
  
  // Serial.printf("日期: %s\n", Future1.date);
  // Serial.printf("温度: %s\n", Future1.temp);
  // Serial.printf("风向: %s\n", Future1.direct);
  // Future1.temp_math();

  // // 未来天气2
  // Future2.date = doc["result"]["future"][2]["date"].as<String>();         // 日期
  // Future2.temp = doc["result"]["future"][2]["temperature"].as<String>();  // 温度
  // //Future2.humi = doc["result"]["future"][2]["humidity"].as<String>();     // 湿度
  // Future2.weather = doc["result"]["future"][2]["weather"].as<String>();   // 天气
  // Future2.day = doc["result"]["future"][2]["day"].as<unsigned int>();     // 白天温度
  // Future2.night = doc["result"]["future"][2]["night"].as<unsigned int>(); // 夜间温度
  // Future2.direct = doc["result"]["future"][2]["direct"].as<char const *>();     // 风向
  
  
  // Serial.printf("日期: %s\n", Future2.date);
  // Serial.printf("温度: %s\n", Future2.temp);
  // Serial.printf("风向: %s\n", Future2.direct);
  // Future2.temp_math();

  // // 未来天气3
  // Future3.date = doc["result"]["future"][3]["date"].as<String>();         // 日期
  // Future3.temp = doc["result"]["future"][3]["temperature"].as<String>();  // 温度
  // //Future3.humi = doc["result"]["future"][3]["humidity"].as<String>();     // 湿度
  // Future3.weather = doc["result"]["future"][3]["weather"].as<String>();   // 天气
  // Future3.day = doc["result"]["future"][3]["day"].as<unsigned int>();     // 白天温度
  // Future3.night = doc["result"]["future"][3]["night"].as<unsigned int>(); // 夜间温度
  // Future3.direct = doc["result"]["future"][3]["direct"].as<char const *>();     // 风向

  // Serial.printf("日期: %s\n", Future3.date);
  // Serial.printf("温度: %s\n", Future3.temp);
  // Serial.printf("风向: %s\n", Future3.direct);
  // Future3.temp_math();

  // // 未来天气4
  // Future4.date = doc["result"]["future"][4]["date"].as<String>();         // 日期
  // Future4.temp = doc["result"]["future"][4]["temperature"].as<String>();  // 温度
  // //Future4.humi = doc["result"]["future"][4]["humidity"].as<String>();     // 湿度
  // Future4.weather = doc["result"]["future"][4]["weather"].as<String>();   // 天气
  // Future4.day = doc["result"]["future"][4]["day"].as<unsigned int>();     // 白天温度
  // Future4.night = doc["result"]["future"][4]["night"].as<unsigned int>(); // 夜间温度
  // Future4.direct = doc["result"]["future"][4]["direct"].as<char const *>();     // 风向
  
  // Serial.printf("日期: %s\n", Future4.date);
  // Serial.printf("温度: %s\n", Future4.temp);
  // Serial.printf("风向: %s\n", Future4.direct);
  // Future4.temp_math();
}
void http_time(){
  // 发送GET请求
  http1.begin(url_time + "?city=" + city1 + "&key=" + key1);

  int httpCode1 = http1.GET();

  // 获取响应状态码
  Serial.printf("HTTP 状态码: %d", httpCode1);

  // 获取响应正文
  String response1 = http1.getString();
  Serial.println("响应数据");
  Serial.println(response1);

  http1.end();

  // 创建 DynamicJsonDocument 对象
  DynamicJsonDocument doc1(1024);

  // 解析 JSON 数据
  deserializeJson(doc1, response1);

  // 从解析后的 JSON 文档中获取值
  String city = doc1["result"]["city"].as<String>();
  String strtime = doc1["result"]["strtime"].as<String>();

  Serial.printf("城市: %s\n", city);

  char p[20];
  int i, j = strtime.length();
  for (i = 0; i < j; i++){
    p[i] = strtime[i];
  }
  p[i] = '\0';
  Serial.printf("时间: %s\n", p); // 2022-12-08 22:39:11
  time_now.Year = atoi(p);
  time_now.Mon = atoi(p + 5);
  time_now.Day = atoi(p + 8);
  time_now.Hour = atoi(p + 11);
  time_now.Min = atoi(p + 14);
  time_now.Second = atoi(p + 17);
  Serial.printf("Year: %d\n", time_now.Year);
  Serial.printf("Mon: %d\n", time_now.Mon);
  Serial.printf("Day: %d\n", time_now.Day);
  Serial.printf("Hour: %d\n", time_now.Hour);
  Serial.printf("Min: %d\n", time_now.Min);
  Serial.printf("Second: %d\n", time_now.Second);
}
void http_api(){
  http_api_state++;
  if(http_api_state>=10){
    http_tianqi();
    http_time();
    http_api_state=0;
  }
}
void httpTask(void *parameter){
  while(1){
    if(WiFi.status() == WL_CONNECTED){
      http_api();
      vTaskDelay(100000);
    }else{
      vTaskDelay(100);
    }
  }
}