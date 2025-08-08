void HandleJPGImages()
{
  if (MODE == BME_MODE)
  {
    if (!pause_display && millis() - lastBMETime >= BMEInterval)
    {
      showBME();
      lastBMETime = millis();
    }
  }
  else if (MODE == HEART_RATE_MONITOR)
  {
    if (!pause_display)
    {
      showHeartRate();
    }
  }
  else if ((MODE == DEFUALT_AP_MODE) || (MODE == USER_MODE) || (MODE == DISPLAY_ONLY))
  {
    if (!pause_display && (millis() - lastImageTime >= imageInterval))
    {
      drawJpeg((const char *)Files[currentImageIndex], 0, 0);
      currentImageIndex = (currentImageIndex + 1) % file_cnt;
      lastImageTime = millis();
    }
  }
}

void HandleMenuItems()
{
  if (millis() - lastDebounceTime > debounceDelay)
  {
    if (upPressed)
    {
      upPressed = false;
      if (pause_display)
      {
        digitalWrite(HAPTIC_PIN, HIGH);
        delay(100);
        digitalWrite(HAPTIC_PIN, LOW);

        // Wrap around to the last item if at the top
        if (selectedItem == 0)
        {
          selectedItem = menuLength - 1;
          topItem = max(0, menuLength - visibleItems);
        }
        else
        {
          selectedItem--;
          if (selectedItem < topItem)
          {
            topItem = selectedItem;
          }
        }

        drawMenu();
      }
      lastDebounceTime = millis();
    }

    if (downPressed)
    {
      downPressed = false;
      if (pause_display)
      {
        digitalWrite(HAPTIC_PIN, HIGH);
        delay(100);
        digitalWrite(HAPTIC_PIN, LOW);

        // Wrap around to the first item if at the bottom
        if (selectedItem == menuLength - 1)
        {
          selectedItem = 0;
          topItem = 0;
        }
        else
        {
          selectedItem++;
          if (selectedItem >= topItem + visibleItems)
          {
            topItem = selectedItem - visibleItems + 1;
          }
        }

        drawMenu();
      }
      lastDebounceTime = millis();
    }

    if (selectPressed)
    {
      selectPressed = false;
      digitalWrite(HAPTIC_PIN, HIGH);
      delay(100);
      digitalWrite(HAPTIC_PIN, LOW);

      if (!pause_display)
      {
        pause_display = true;
        selectedItem = 0;
        topItem = 0;
        drawMenu();
      }
      else
      {
        handleMenuSelect(selectedItem);
      }

      lastDebounceTime = millis();
    }
  }
}

void drawMenu() 
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);

  // Draw menu items
  for (int i = 0; i < visibleItems; i++) {
    int itemIndex = topItem + i;
    if (itemIndex >= menuLength) break;

    if (itemIndex == selectedItem) {
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }

    tft.setCursor(10, 10 + i * 24);
    tft.println(menuItems[itemIndex]);
  }

  // === Draw Scrollbar ===
  int scrollbarX = tft.width() - 6; // Right side
  int scrollbarY = 10;
  int scrollbarH = visibleItems * 24;
  int trackHeight = scrollbarH;
  int trackTop = scrollbarY;

  tft.fillRect(scrollbarX, trackTop, 4, trackHeight, TFT_DARKGREY); // Track

  // Calculate thumb (indicator) height & position
  int thumbHeight = max((visibleItems * trackHeight) / menuLength, 8); // Minimum size
  int thumbPos = (selectedItem * (trackHeight - thumbHeight)) / (menuLength - 1);

  tft.fillRect(scrollbarX, trackTop + thumbPos, 4, thumbHeight, TFT_WHITE); // Thumb
}

void handleMenuSelect(int index)
{
  if (index == 0)
  {
    Serial.println("Going to Back...");
    pause_display = false;
    if (MODE == BME_MODE)
    {
      tft.fillScreen(TFT_BLACK);
      drawGaugeFrame(TEMP_X, TEMP_Y, "Temp", TFT_RED);
      drawGaugeFrame(HUM_X, HUM_Y, "Hum", TFT_CYAN);
    }
    else if (MODE == HEART_RATE_MONITOR)
    {
      InitHeartRate();
    }
//    else if ((MODE == DEFUALT_AP_MODE) || (MODE == USER_MODE) || (MODE == DISPLAY_ONLY))
//    {
//      // Pass
//    }
  }
  else if (index == 1)
  {
    Serial.println("Switching to Defualt AP...");
    MODE = DEFUALT_AP_MODE;
    save_credentials();
    ESP.restart();
  }
  else if (index == 2)
  {
    Serial.println("Switching to User Mode...");
    MODE = USER_MODE;
    save_credentials();
    ESP.restart();
  }
  else if (index == 3)
  {
    Serial.println("Switching to Only Display Mode...");
    MODE = DISPLAY_ONLY;
    save_credentials();
    ESP.restart();
  }
  else if (index == 4)
  {
    Serial.println("Switching to BME_MODE...");
    MODE = BME_MODE;
    save_credentials();
    ESP.restart();
  }
  else if (index == 5)
  {
    Serial.println("Switching to HEART_RATE_MONITOR...");
    MODE = HEART_RATE_MONITOR;
    save_credentials();
    ESP.restart();
  }
}

void showIP(String ip)
{
  ip = "IP: " + ip;
  int16_t xCenter, yCenter;
  int textWidth = tft.textWidth(ip);
  int textHeight = 10 * tft.textsize; // Assuming text size 1 → 10px height

  xCenter = (tft.width() - textWidth) / 2;
  yCenter = (tft.height() - textHeight) / 2;

  touchDetachInterrupt(TOUCH_SELECT);
  pause_display = true;

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(28, 30);
  tft.print("Connected");
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.setCursor(xCenter, yCenter);
  tft.print(ip);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setCursor(75, 96);
  tft.print("OK");
  for (int i = 0; i < 45; i++)
  {
    if (touchRead(TOUCH_SELECT) < TOUCH_INTERRUPT_THRESHOLD)
    {
      break;
    }
    delay(100);
  }
  pause_display = false;
  touchAttachInterrupt(TOUCH_SELECT, onTouchSelect, TOUCH_INTERRUPT_THRESHOLD);
  digitalWrite(HAPTIC_PIN, HIGH);
  delay(100);
  digitalWrite(HAPTIC_PIN, LOW);
}
