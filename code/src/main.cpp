#include <Arduino.h>
#include <Wire.h>
#include <VL6180X.h>
#include <U8g2lib.h>

#define VL6180X_ADDRESS 0x29
#define OLED_ADDRESS 0x3C

int min_distance = 15;
int max_distance = 150;

VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

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
}

void loop() {
  static unsigned long last_50ms = 0;
  static unsigned long last_100ms = 0;
  unsigned long currentMillis = millis();
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
