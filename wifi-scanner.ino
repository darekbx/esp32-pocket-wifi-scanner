int wifiItemIndex = 0;
int wifiResultsCount = 0;

void WiFiloop() {
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
}

void scanWifi() {
  M5.Lcd.fillRect(0, batteryLevelOffset + 6, 80, 126, BLACK);
  wifiItemIndex = 0;
  
  int maxLines = 9;
  int topOffset = 6;
  int lineOffset = 11;
  int maxWidth = 12;
  
  M5.Lcd.setCursor(1, batteryLevelOffset + topOffset, 1);
  M5.Lcd.print("Scanning...");
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  int n = WiFi.scanNetworks(false, true, true);
  wifiResultsCount = n;
  M5.Lcd.fillRect(0, batteryLevelOffset + topOffset, 80, 8, BLACK);
  for (int i = 0; i < min(n, maxLines); ++i) {
    String ssid = WiFi.SSID(i);
    M5.Lcd.setCursor(1, (batteryLevelOffset + topOffset) + i * lineOffset, 1);
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
    M5.Lcd.setCursor(1, (batteryLevelOffset + topOffset) + (maxLines) * lineOffset, 1);
    M5.Lcd.print("...");
    M5.Lcd.setCursor(1, 136, 1);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.printf("Total: %d", n);
  }
  
  WiFi.mode(WIFI_OFF);
}

void scanWifiDetails() {
  M5.Lcd.fillRect(0, batteryLevelOffset + 6, 80, 126, BLACK);
  
  String results[3] = { "SSID:", "RSSI:", "Channel:" };
  int top = 0;
  int start = batteryLevelOffset + 6;
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
