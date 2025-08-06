# ESP32 Digital Photo Frame with Health & Weather Monitoring

![Project Image](./images/photo-frame.jpg)\
*(Add your actual project photo here)*

## 📌 Overview

This project is a **Wi-Fi-enabled Digital Photo Frame** built using
**ESP32** and a **1.8" ST7735 TFT display**. It allows users to connect
to the device via `esp32.local` and **upload or remove images** that
will be displayed as a slideshow.

Additionally, it features:\
- 🌡️ **Weather Mode:** Supports an **I2C-based BME280 sensor** to
display temperature and humidity with a gauge-style UI.\
- ❤️ **Health Mode:** Supports a **MAX30102 pulse oximeter sensor** to
measure and display heart rate in real time.

------------------------------------------------------------------------

## ✨ Features

-   📷 **Digital Photo Frame:** Upload and remove images over Wi-Fi\
-   🌐 **Web Interface:** Access via `esp32.local` for image management\
-   🌡️ **Weather Monitoring:** Real-time temperature and humidity from
    BME280\
-   ❤️ **Health Monitoring:** Real-time heart rate display from
    MAX30102\
-   🎨 **Gauge-like UI:** Smooth graphical display for weather and heart
    rate\
-   🔌 **I2C Sensor Interface:** Easily connect supported sensors\
-   ⚡ **Low Power Mode:** Optimized display and Wi-Fi handling

------------------------------------------------------------------------

## 🛠️ Hardware Requirements

-   ESP32 Development Board\
-   1.8" ST7735 TFT Display (128x160)\
-   BME280 Temperature & Humidity Sensor *(optional)*\
-   MAX30102 Pulse Oximeter Sensor *(optional)*\
-   Power Supply (5V via USB or battery pack)

------------------------------------------------------------------------

## 📂 Project Structure

    📦 ESP32-Digital-Photo-Frame
     ┣ 📂 src
     ┃ ┣ main.cpp
     ┃ ┗ ...
     ┣ 📂 data
     ┃ ┗ index.html        # Web interface files
     ┣ 📂 images           # Example images
     ┣ 📂 include
     ┣ 📜 platformio.ini   # PlatformIO configuration
     ┣ 📜 README.md
     ┗ 📜 LICENSE

------------------------------------------------------------------------

## 🚀 Getting Started

### 1️⃣ Flashing the Firmware

1.  Clone this repository

    ``` bash
    git clone https://github.com/your-username/esp32-digital-photo-frame.git
    cd esp32-digital-photo-frame
    ```

2.  Open with **PlatformIO** or **Arduino IDE**\

3.  Update **Wi-Fi credentials** in `config.h`\

4.  Upload the code to ESP32

------------------------------------------------------------------------

### 2️⃣ Using the Web Interface

-   After boot, connect your device to the same Wi-Fi network\

-   Open a browser and go to:

        http://esp32.local

-   Upload or remove images easily via the UI

------------------------------------------------------------------------

### 3️⃣ Sensor Modes

-   **Weather Mode:** Connect a **BME280** sensor via I2C to see live
    temperature and humidity readings\
-   **Health Mode:** Connect a **MAX30102** sensor to measure and
    display heart rate

------------------------------------------------------------------------

## 📸 Screenshots / Videos

### 🖼️ Digital Photo Frame

![Photo Frame Mode](./images/photo-frame-display.jpg)

### 🌡️ Weather Monitoring

![Weather Mode](./images/weather-mode.jpg)

### ❤️ Health Monitoring

![Health Mode](./images/health-mode.jpg)

*(Add more screenshots and demo videos here)*

------------------------------------------------------------------------

## 🧩 Future Enhancements

-   Add SpO₂ display for MAX30102\
-   Cloud sync for photo uploads\
-   Support for more display sizes\
-   Battery status monitoring

------------------------------------------------------------------------

## 📝 License

This project is licensed under the MIT License.
