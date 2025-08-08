float deg2rad(float deg)
{
  return deg * PI / 180.0;
}

// Draw gauge ticks from 45° to 315° (270° arc)
void drawGaugeTicks(int cx, int cy)
{
  for (int i = 0; i <= 100; i += 10)
  {
    float angle = map(i, 0, 100, 45, 315);
    float rad = deg2rad(angle);

    int x0 = cx + cos(rad) * (RADIUS - 1);
    int y0 = cy + sin(rad) * (RADIUS - 1);
    int x1 = cx + cos(rad) * (RADIUS - 6);
    int y1 = cy + sin(rad) * (RADIUS - 6);
    tft.drawLine(x0, y0, x1, y1, TFT_LIGHTGREY);

    if (i % 20 == 0)
    {
      int xt = cx + cos(rad) * (RADIUS - 12);
      int yt = cy + sin(rad) * (RADIUS - 12);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(1);
      tft.drawString(String(i), xt, yt);
    }
  }
}

void drawNeedleLine(int cx, int cy, float value, float minVal, float maxVal, float &lastVal, uint16_t color1, uint16_t color2)
{
  // Erase previous needle
  if (lastVal >= 0)
  {
    float lastAngle = map(lastVal, minVal, maxVal, 45, 315);
    int lx0 = cx + cos(deg2rad(lastAngle)) * (RADIUS - 5);
    int ly0 = cy + sin(deg2rad(lastAngle)) * (RADIUS - 5);

    int lx1 = cx + cos(deg2rad(lastAngle - 90)) * 2;
    int ly1 = cy + sin(deg2rad(lastAngle - 90)) * 2;

    int lx2 = cx + cos(deg2rad(lastAngle + 90)) * 2;
    int ly2 = cy + sin(deg2rad(lastAngle + 90)) * 2;

    int lx3 = cx + cos(deg2rad(lastAngle + 180)) * 10;
    int ly3 = cy + sin(deg2rad(lastAngle + 180)) * 10;
    // Upper
    tft.fillTriangle(lx0, ly0, cx, cy, lx1, ly1, TFT_BLACK);
    tft.fillTriangle(lx0, ly0, cx, cy, lx2, ly2, TFT_BLACK);
    tft.drawLine(lx0, ly0, lx3, ly3, TFT_BLACK);
    // Lower
    tft.fillTriangle(lx3, ly3, cx, cy, lx1, ly1, TFT_BLACK);
    tft.fillTriangle(lx3, ly3, cx, cy, lx2, ly2, TFT_BLACK);
  }

  // Draw new needle
  float angle = map(value, minVal, maxVal, 45, 315);
  int x0 = cx + cos(deg2rad(angle)) * (RADIUS - 5);
  int y0 = cy + sin(deg2rad(angle)) * (RADIUS - 5);

  int x1 = cx + cos(deg2rad(angle - 90)) * 2;
  int y1 = cy + sin(deg2rad(angle - 90)) * 2;

  int x2 = cx + cos(deg2rad(angle + 90)) * 2;
  int y2 = cy + sin(deg2rad(angle + 90)) * 2;

  int x3 = cx + cos(deg2rad(angle + 180)) * 10;
  int y3 = cy + sin(deg2rad(angle + 180)) * 10;
  // Upper
  tft.fillTriangle(x0, y0, cx, cy, x1, y1, color1);
  tft.fillTriangle(x0, y0, cx, cy, x2, y2, color2);
  tft.drawLine(x0, y0, x3, y3, color1);
  // Lower
  tft.fillTriangle(x3, y3, cx, cy, x1, y1, color1);
  tft.fillTriangle(x3, y3, cx, cy, x2, y2, color2);

  // Center cap
  tft.fillCircle(cx, cy, 3, TFT_WHITE);
  lastVal = value;
}

void drawGaugeFrame(int x, int y, const char *label, uint16_t labelColor)
{
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(labelColor, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString(label, x, y - RADIUS - 30);
  tft.drawCircle(x, y, RADIUS, TFT_WHITE);
  drawGaugeTicks(x, y);
}

void BME_Init()
{
  Wire.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  if (bme280.begin(BME280_ADDRESS))
  {
    Serial.println(F("BME280 found"));
    bme280.setSampling(
      Adafruit_BME280::MODE_NORMAL,       // Forced mode
      Adafruit_BME280::SAMPLING_X2,       // Temp oversampling x2 (like your IDF config)
      Adafruit_BME280::SAMPLING_X16,      // Pressure oversampling x16
      Adafruit_BME280::SAMPLING_NONE,     // Humidity skipped
      Adafruit_BME280::FILTER_X16,        // Filter coeff 16
      Adafruit_BME280::STANDBY_MS_10     // Standby 1ms
    );
    select_sensor = BME280;
  }
  else if (bme680.begin())
  {
    Serial.println(F("BME680 found"));
    select_sensor = BME680;
  }
  else
  {
    Serial.println(F("No BME found"));
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("BME not found!");
    delay(2000);
    MODE = DISPLAY_ONLY;
    save_credentials();
    ESP.restart();
  }
  if (select_sensor != NONE)
  {
    drawGaugeFrame(TEMP_X, TEMP_Y, "Temp", TFT_RED);
    drawGaugeFrame(HUM_X, HUM_Y, "Hum", TFT_CYAN);
  }
}

void showBME()
{
  if ((select_sensor == BME280) && (!checkSensor()))
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("BME280 not found!");
    delay(2000);
    MODE = DISPLAY_ONLY;
    save_credentials();
    ESP.restart();
  }
  else if ((select_sensor == BME680) && (!checkSensor()))
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("BME680 not found!");
    delay(2000);
    MODE = DISPLAY_ONLY;
    save_credentials();
    ESP.restart();
  }

  float temp;
  float hum;

  if (select_sensor == BME280)
  {
    temp = bme280.readTemperature();
    hum = bme280.readHumidity();
  }
  else if (select_sensor == BME680)
  {
    temp = bme680.readTemperature();
    hum = bme680.readHumidity();
  }

  Serial.println("Temp: " + String(temp) + " C\t Hum: " + String(hum) + " %");

  temp = constrain(temp, 0, 100);
  hum = constrain(hum, 0, 100);

  drawNeedleLine(TEMP_X, TEMP_Y, temp, 0, 100, lastTemp, TFT_RED, tft.color565(200, 0, 0));
  drawNeedleLine(HUM_X, HUM_Y, hum, 0, 100, lastHum, TFT_CYAN, tft.color565(0, 200, 200));

  // Show values above each gauge
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.fillRect(TEMP_X - 30, TEMP_Y - 50, 60, 16, TFT_BLACK);
  tft.drawString(String(temp, 1) + " C", TEMP_X, TEMP_Y - 43);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.fillRect(HUM_X - 30, HUM_Y - 50, 60, 16, TFT_BLACK);
  tft.drawString(String((int)hum) + "%", HUM_X, HUM_Y - 43);
}

bool checkSensor()
{
  if (select_sensor == BME280)
  {
    Wire.beginTransmission (0x76);
    return (Wire.endTransmission () == 0);
  }
  else if (select_sensor == BME680)
  {
    Wire.beginTransmission (0x77);
    return (Wire.endTransmission () == 0);
  }
  else
  {
    return false;
  }
}
