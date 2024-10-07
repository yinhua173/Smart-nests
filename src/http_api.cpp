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


/*时间戳转换为标准时间*/
typedef struct times{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Second;
}Times;
Times time_now;//定义时间结构体
void http_api(){
  http_api_state++;
  if(http_api_state>=10){
    // 发送GET请求
    http.begin(url+"?city="+city+"&key="+key);

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
    unsigned int temp = doc["result"]["realtime"]["temperature"].as<unsigned int>();
    String info = doc["result"]["realtime"]["info"].as<String>();
    int aqi = doc["result"]["realtime"]["aqi"].as<int>();
    
    Serial.printf("温度: %d\n", temp);
    Serial.printf("天气: %s\n", info);
    Serial.printf("空气指数: %d\n", aqi);


    // 发送GET请求
    http1.begin(url_time+"?city="+city1+"&key="+key1);

    httpCode = http1.GET();

    // 获取响应状态码
    Serial.printf("HTTP 状态码: %d", httpCode);

    // 获取响应正文
    response = http1.getString();
    Serial.println("响应数据");
    Serial.println(response);

    http1.end();

    // 创建 DynamicJsonDocument 对象
    DynamicJsonDocument doc1(1024);

    // 解析 JSON 数据
    deserializeJson(doc1, response);

    // 从解析后的 JSON 文档中获取值
    //unsigned int temp = doc["result"]["city"].as<unsigned int>();
    String city = doc1["result"]["city"].as<String>();
    //String timeZone = doc1["result"]["timeZone"].as<String>();
    String strtime = doc1["result"]["strtime"].as<String>();
    //int timestamp = doc1["result"]["timestamp"].as<int>();
    
    //Serial.printf("温度: %d\n", temp);
    Serial.printf("城市: %s\n", city);
    //Serial.printf("时区: %s\n", timeZone);
    
    char p[20];
    int i,j=strtime.length();
    for(i=0;i<j;i++){
      p[i] = strtime[i];
    }
    p[i] = '\0';
    Serial.printf("时间: %s\n",p);//2022-12-08 22:39:11
    time_now.Year = atoi(p);
    time_now.Mon = atoi(p+5);
    time_now.Day = atoi(p+8);
    time_now.Hour = atoi(p+11);
    time_now.Min = atoi(p+14);
    time_now.Second = atoi(p+17);
    Serial.printf("Year: %d\n",time_now.Year);
    Serial.printf("Mon: %d\n",time_now.Mon);
    Serial.printf("Day: %d\n",time_now.Day);
    Serial.printf("Hour: %d\n",time_now.Hour);
    Serial.printf("Min: %d\n",time_now.Min);
    Serial.printf("Second: %d\n",time_now.Second);
    http_api_state=0;
  }
  
}
void httpTask(void *parameter){
  
  while(1){
    if(WiFi.status() == WL_CONNECTED){
      http_api();
      vTaskDelay(10000);
    }else{
      vTaskDelay(100);
    }
  }
}