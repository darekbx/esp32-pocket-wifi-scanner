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

int wifiItemIndex = 0;
int wifiResultsCount = 0;

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
  
  pinMode(M5_BUTTON_HOME, INPUT_PULLUP);
  
  M5.Axp.ScreenBreath(8);
}

void loop() {
  M5.Lcd.setTextColor(WHITE);
  
  displayBatteryLevel();
  displayTemperture();

  M5.Lcd.setCursor(0, 60, 2);
  
  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    scanWifi();
  } 
  
  if (digitalRead(BUTTON_B_PIN) == LOW) {
    scanWifiDetails();
    if (wifiItemIndex < wifiResultsCount) {
      wifiItemIndex++;
    } else {
      wifiItemIndex = 0;
    }
  }

  delay(150);
}

void scanWifi() {
  M5.Lcd.fillRect(0, batteryLevelHeight + 6, 80, 126, BLACK);
  wifiItemIndex = 0;
  
  int maxLines = 9;
  int topOffset = 6;
  int lineOffset = 11;
  int maxWidth = 12;
  
  M5.Lcd.setCursor(1, batteryLevelHeight + topOffset, 1);
  M5.Lcd.print("Scanning...");
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  int n = WiFi.scanNetworks(false, true, true);
  wifiResultsCount = n;
  M5.Lcd.fillRect(0, batteryLevelHeight + topOffset, 80, 8, BLACK);
  for (int i = 0; i < min(n, maxLines); ++i) {
    String ssid = WiFi.SSID(i);
    M5.Lcd.setCursor(1, (batteryLevelHeight + topOffset) + i * lineOffset, 1);
    if (ssid.length() > maxWidth) {
      M5.Lcd.printf("%s...", ssid.substring(0, maxWidth - 3));
    } else if (ssid.length() == 0) {
      M5.Lcd.setTextColor(RED);
      M5.Lcd.print("[n\\a]");
      M5.Lcd.setTextColor(WHITE);
    } else {
      M5.Lcd.print(ssid);
    }
  }

  if (n > maxLines) {
    M5.Lcd.setCursor(1, (batteryLevelHeight + topOffset) + (maxLines) * lineOffset, 1);
    M5.Lcd.print("...");
    M5.Lcd.setCursor(1, 136, 1);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.printf("Total: %d", n);
  }

  
  WiFi.mode(WIFI_OFF);
}

void scanWifiDetails() {
  M5.Lcd.fillRect(0, batteryLevelHeight + 6, 80, 126, BLACK);
  
  String results[3] = { "SSID:", "RSSI:", "Channel:" };
  int top = 0;
  int start = batteryLevelHeight + 6;
  for (int i = 0; i < 3; i++) {
    M5.Lcd.setCursor(1, start + top, 1);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print(results[i]);
    top += 11;
    M5.Lcd.setCursor(1, start + top, 1);
    M5.Lcd.setTextColor(GREEN);
    if (i == 0) {
      String ssid = WiFi.SSID(wifiItemIndex);
      if (ssid.length() == 0) {
        M5.Lcd.setTextColor(RED);
        M5.Lcd.print("[n\\a]");
        M5.Lcd.setTextColor(WHITE);
      } else {
        M5.Lcd.print(WiFi.SSID(wifiItemIndex));
      }
    } else if (i == 1) {
      M5.Lcd.print(WiFi.RSSI(wifiItemIndex));
    } else if (i == 2) {
      M5.Lcd.print(WiFi.channel(wifiItemIndex));
    }
    top += 14;
    if (i == 0) {
      top += 22;
    }
  }
}

void displayTemperture() {
  if (temperatureEnabled == 0) {
    return;
  }
  M5.IMU.getTempData(&temp);
  
  M5.Lcd.fillRect(0, 148, 80, 160, BLACK);
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

  M5.Lcd.fillRect(batteryLevelBoxWidth + 4, 0, 80, 12, BLACK);
  M5.Lcd.setCursor(batteryLevelBoxWidth + 4, 4, 1);
  M5.Lcd.printf("%.3fV", vbat);  
}
