# ESP32 Digital Photo Frame with Health & Weather Monitoring

</br><img src="https://github.com/shuvabratadey/Digital-Photo_Frame/blob/main/pictures/IMG_1.jpg" width="400"/>
<img src="https://github.com/shuvabratadey/Digital-Photo_Frame/blob/main/pictures/IMG_4.jpg" width="400"/>

## 📌 Overview

This project is a **Wi-Fi-enabled Digital Photo Frame** built using
**ESP32** and a **1.8" ST7735 TFT display**. It allows users to connect
to the device via `esp32.local` and **upload or remove images** that
will be displayed as a slideshow.

Additionally, it features:
- 🌡️ **Weather Mode:** Supports an **I2C-based BME280 sensor** to
display temperature and humidity with a gauge-style UI.
- ❤️ **Health Mode:** Supports a **MAX30102 pulse oximeter sensor** to
measure and display heart rate in real time.

------------------------------------------------------------------------

## ✨ Features

-   📷 **Digital Photo Frame:** Upload and remove images over Wi-Fi
-   🌐 **Web Interface:** Access via `esp32.local` for image management
-   🌡️ **Weather Monitoring:** Real-time temperature and humidity from
    BME280
-   ❤️ **Health Monitoring:** Real-time heart rate display from
    MAX30102
-   🎨 **Gauge-like UI:** Smooth graphical display for weather and heart
    rate
-   🔌 **I2C Sensor Interface:** Easily connect supported sensors
-   ⚡ **Low Power Mode:** Optimized display and Wi-Fi handling

------------------------------------------------------------------------

## 🛠️ Hardware Requirements

-   ESP32 Development Board
-   1.8" ST7735 TFT Display (128x160)
-   BME280 Temperature & Humidity Sensor *(optional)*
-   MAX30102 Pulse Oximeter Sensor *(optional)*
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

2.  Open with **PlatformIO** or **Arduino IDE**

3.  Update **Wi-Fi credentials** in `config.h`

4.  Upload the code to ESP32

------------------------------------------------------------------------

### 2️⃣ Using the Web Interface

-   After boot, connect your device to the same Wi-Fi network

-   Open a browser and go to:

        http://esp32.local

-   Upload or remove images easily via the UI

------------------------------------------------------------------------

### 3️⃣ Sensor Modes

-   **Weather Mode:** Connect a **BME280** sensor via I2C to see live
    temperature and humidity readings
-   **Health Mode:** Connect a **MAX30102** sensor to measure and
    display heart rate

------------------------------------------------------------------------

## 📸 Screenshots / Videos

### 🖼️ Digital Photo Frame


![Photo Frame Mode](https://github.com/shuvabratadey/Digital-Photo_Frame/blob/main/pictures/Digital_Photo_Frame.png)

### 🌡️ Weather Monitoring



https://github.com/user-attachments/assets/a6aec441-a87f-4112-b910-02521dc4f6c6



### ❤️ Health Monitoring
<div align="center">
<table>
  <tr>
    <td>
      <video autoplay muted loop src="https://github.com/user-attachments/assets/08b8649f-e81c-403b-ba0d-ddd3171b3d4c"></video>
    </td>
    <td>
    <img src="https://github.com/shuvabratadey/esp32-Smart-Watch/blob/main/pictures/Heart_Rate_App.jpg?raw=true" width="300"/>
    </td>
  </tr>
</table>
</div>

## 🧩 Future Enhancements

-   Add SpO₂ display for MAX30102
-   Cloud sync for photo uploads
-   Support for more display sizes
-   Battery status monitoring

------------------------------------------------------------------------
