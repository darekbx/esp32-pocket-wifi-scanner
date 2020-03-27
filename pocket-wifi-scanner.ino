#include <M5StickC.h>
#include <WiFi.h>

#define temperatureEnabled 1

const float batteryLevel100 = 4.2;
const float batteryLevel0 = 3.6;

int batteryLevelBoxWidth = 38;
int batteryLevelWidth = batteryLevelBoxWidth - 4;
int batteryLevelHeight = 12;

float temp = 0;
int disCharge;
int bat;

static uint16_t color16(uint16_t r, uint16_t g, uint16_t b) {
  uint16_t _color;
  _color = (uint16_t)(r & 0xF8) << 8;
  _color |= (uint16_t)(g & 0xFC) << 3;
  _color |= (uint16_t)(b & 0xF8) >> 3;
  return _color;
}

void setup() {
  M5.begin();
  if (temperatureEnabled == 1) {
    M5.IMU.Init();
  }
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  pinMode(M5_BUTTON_HOME, INPUT_PULLUP);
  
  M5.Axp.ScreenBreath(8);
}

void loop() {
  //M5.Lcd.fillScreen(BLACK);
  
  displayBatteryLevel();
  displayTemperture();
  //vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  //M5.Lcd.setCursor(0, 0, 2);
  //M5.Lcd.printf("%.3fV\r\n", vbat);
  
  //bat = M5.Axp.GetPowerbatData()*1.1*0.5/1000;
  //M5.Lcd.setCursor(0, 15, 2);
  //M5.Lcd.printf("%dmW\r\n", bat);

  //disCharge = M5.Axp.GetIdischargeData() / 2;
 // M5.Lcd.setCursor(0, 30, 2);
  //M5.Lcd.printf("%dmA\r\n", disCharge);

  M5.Lcd.setCursor(0, 60, 2);
  
  if(digitalRead(M5_BUTTON_HOME) == LOW){
    M5.Lcd.printf("Pressed.");
  } else{ 
    M5.Lcd.printf("Released.");
  }

  delay(500);
}

void displayTemperture() {
  if (temperatureEnabled == 0) {
    return;
  }
  M5.IMU.getTempData(&temp);
  
  M5.Lcd.drawLine(0, 145, 80, 145, WHITE);
  M5.Lcd.setCursor(33, 150);
  M5.Lcd.printf("%.2f", temp);
  M5.Lcd.setCursor(70, 150);
  M5.Lcd.printf("C", temp);
  M5.Lcd.drawCircle(66, 150, 2, WHITE);
}

void displayBatteryLevel() {
  /*
  4.2 volts 100%
  4.1 about 90%
  4.0 about 80%
  3.9 about 60%
  3.8 about 40%
  3.7 about 20%
  3.6 empty
  */
  float vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  float diff = vbat - batteryLevel0;
  float diffMax = batteryLevel100 - batteryLevel0;
  float percent = ceil(diff / diffMax * 100);
  int level = min(max(int(batteryLevelWidth * (percent / 100)), 4), batteryLevelWidth);

  M5.Lcd.drawRect(1, 1, batteryLevelBoxWidth, batteryLevelHeight, WHITE);

  if (percent <= 25) {
    M5.Lcd.fillRect(3, 3, batteryLevelWidth, batteryLevelHeight - 4, RED);
  } else {
    M5.Lcd.fillRect(3, 3, batteryLevelWidth, batteryLevelHeight - 4, GREEN);
  }
  M5.Lcd.fillRect(3 + level, 3, batteryLevelWidth - level, batteryLevelHeight - 4, BLACK);

  M5.Lcd.setCursor(batteryLevelBoxWidth + 4, 4, 1);
  M5.Lcd.printf("%.3fV", vbat);  
}
