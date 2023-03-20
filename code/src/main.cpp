#include <Arduino.h>
#include <Wire.h>
#include <VL6180X_WE.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define VL6180X_ADDRESS 0x29
#define OLED_ADDRESS 0x3C

const char* ssid = "ESP-SSID";
const char* password = "my-esp32-password";
int min_distance = 15;
int max_distance = 150;

AsyncWebServer server(80);

VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

void setupWiFi() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}




void printIdentification(struct VL6180xIdentification *temp){
  Serial.print("设备 ID = ");
  Serial.println(temp->idModel);

  Serial.print("设备 Rev = ");
  Serial.print(temp->idModelRevMajor);
  Serial.print(".");
  Serial.println(temp->idModelRevMinor);

  Serial.print("修订 Rev = ");
  Serial.print(temp->idModuleRevMajor);
  Serial.print(".");
  Serial.println(temp->idModuleRevMinor);  

  Serial.print("生产日期 = ");
  Serial.print((temp->idDate >> 3) & 0x001F);
  Serial.print("/");
  Serial.print((temp->idDate >> 8) & 0x000F);
  Serial.print("/1");
  Serial.print((temp->idDate >> 12) & 0x000F);
  Serial.print(" 阶段: ");
  Serial.println(temp->idDate & 0x0007);

  Serial.print("生产时间 (s)= ");
  Serial.println(temp->idTime * 2);
  Serial.println();
  Serial.println();
}

void delay_ms(int ms) {
  unsigned long startMillis = millis();
  while (millis() - startMillis < ms);
}


void updateDistanceHandler(AsyncWebServerRequest *request) {
  if (request->hasParam("min_distance")) {
    min_distance = request->getParam("min_distance")->value().toInt();
  }
  if (request->hasParam("max_distance")) {
    max_distance = request->getParam("max_distance")->value().toInt();
  }
  request->send(200, "text/plain", "Updated values: min_distance=" + String(min_distance) + ", max_distance=" + String(max_distance));
}


void setup() {
  Serial.begin(9600); 
  Wire.begin();

  u8g2.begin();
  u8g2.enableUTF8Print();

  delay(1000); 

  sensor.getIdentification(&identification); 
  printIdentification(&identification); 

  if(sensor.VL6180xInit() != 0){
    Serial.println("VL6180初始化失败"); 
  }; 

  sensor.VL6180xDefautSettings();   
  delay(1000); 

  setupWiFi();

  // 配置更新距离的路由
  server.on("/updateDistance", HTTP_POST, updateDistanceHandler);
  server.begin();
}

void loop() {
  static unsigned long last_50ms = 0;
  static unsigned long last_100ms = 0;
  unsigned long currentMillis = millis();


  if (WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }

  // 每隔一段时间检查一次表单参数是否有更新
  static unsigned long last_check_time = 0;
  if (millis() - last_check_time > 5000) {
    last_check_time = millis();
    if (WiFi.status() != WL_CONNECTED) {
      setupWiFi();
    }
  Serial.println(min_distance);
  Serial.println(max_distance);
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy14_t_gb2312b); 
  u8g2.setFontDirection(0);

  int distance = sensor.getDistance();

  if (distance >= min_distance && distance <= max_distance) {
    u8g2.setCursor(0, 15);
    u8g2.print("光照：");
    u8g2.print(sensor.getAmbientLight(GAIN_10));
    u8g2.print(" Lux");
    u8g2.setCursor(0, 30);
    u8g2.print("距离：");
    u8g2.print(distance);
    u8g2.print(" mm");
  } else if (distance < min_distance) {
    u8g2.setCursor(0, 15);
    u8g2.print("距离过近");
  } else {
    u8g2.setCursor(0, 15);
    u8g2.print("距离过远");
  }


  u8g2.sendBuffer();

  Serial.print("环境光级别 (Lux) = ");
  Serial.println(sensor.getAmbientLight(GAIN_10));
  Serial.print("测量距离 (mm) = ");
  Serial.println(sensor.getDistance());

  if (currentMillis - last_50ms >= 50) {
    last_50ms = currentMillis;
    Serial.println("Delay 50ms");
    delay_ms(50);
  } 
};
