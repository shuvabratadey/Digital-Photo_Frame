void InitHeartRate()
{
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    tft.setCursor(10, 10);
    tft.setTextSize(1);
    tft.println("Heart_Rate not found!");
    delay(2000);
    MODE = DISPLAY_ONLY;
    save_credentials();
    ESP.restart();
  }
  else
  {
    particleSensor.setup(); //Configure sensor with default settings
    particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

    showImg();
    preHeartBeat = "";
  }
}

void showHeartRate()
{
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextSize(1);
    tft.println("Heart_Rate not found!");
    delay(2000);
    MODE = DISPLAY_ONLY;
    save_credentials();
    ESP.restart();
  }
  HeartBeatData();
}

void HeartBeatData()
{
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true) {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  if (irValue < 50000)
  {
    HeartBeat = " NF";
  }
  else
  {
    if (cnt > 24)
    {
      uint8_t pulse = pulseAvg / 25;
      if (pulse < 10)
      {
        HeartBeat = "  " + String(pulse);
      }
      else if (pulse < 100)
      {
        HeartBeat = " " + String(pulse);
      }
      else
      {
        HeartBeat = String(pulse);
      }
      pulseAvg = 0;
      cnt = 0;
    }
    else
    {
      pulseAvg += beatAvg;
      cnt++;
    }
  }
  if (preHeartBeat != HeartBeat)
  {
    String temp = HeartBeat;
    tft.fillRect(32, 79, 54, 35, 0x0006);
    tft.setTextSize(3);
    tft.setTextColor(TFT_RED, 0x0006);
    tft.setCursor(33, 80);
    tft.print(HeartBeat);
    temp.replace(" ", "");
    SerialBT.println(temp);
    preHeartBeat = HeartBeat;
  }
}

void showImg()
{
  for (int y = 0; y < IMG_HEIGHT; y++) {
    for (int x = 0; x < IMG_WIDTH; x++) {
      tft.drawPixel(x, y, heartbeat_image[y][x]);
    }
  }
}
