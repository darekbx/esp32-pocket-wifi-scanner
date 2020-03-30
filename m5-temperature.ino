
void displayTemperture() {
  M5.Lcd.drawLine(0, 145, 80, 145, WHITE);
  
  if (temperatureEnabled == 0) {
    M5.Lcd.setCursor(1, 150, 1);
    return;
  }
  float temp;
  M5.IMU.getTempData(&temp);
  
  M5.Lcd.fillRect(0, 148, 80, 160, BLACK);
  M5.Lcd.setCursor(33, 150);
  M5.Lcd.printf("%.2f", temp);
  M5.Lcd.setCursor(70, 150);
  M5.Lcd.printf("C", temp);
  M5.Lcd.drawCircle(66, 150, 2, WHITE);
}
