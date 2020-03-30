#include <M5StickC.h>
#include <WiFi.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define bluetoothEnabled 0
#define temperatureEnabled 1

int batteryLevelOffset = 12;

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

  if (bluetoothEnabled == 1) {
    initBLE();
  }
  
  pinMode(M5_BUTTON_HOME, INPUT_PULLUP);

  M5.Axp.ScreenBreath(8);
}

void loop() {
  M5.Lcd.setTextColor(WHITE);
  
  displayBatteryLevel();
  displayTemperture();
  
  if (bluetoothEnabled == 1) {
    M5BLEloop();
  }
  WiFiloop();
  
  delay(150);
}
