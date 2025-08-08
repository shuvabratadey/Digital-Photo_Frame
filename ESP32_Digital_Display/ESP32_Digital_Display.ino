/*
  Select ➡ Flash Size: 8M
  Select ➡ Partition Scheme: Custom 8M with spiffs
  Upload this code.
  Then Click ESP32 Sketch Data Upload.

  Note ➡ This code supports only jpeg format not jpg
      Image Resolution ➡ 130 x 161 (Width x HIGHT)
      Convert JPG to JPEG ➡ https://www.freeconvert.com/jpg-to-jpeg
*/

#include <TFT_eSPI.h>
#include "FS.h"
#include "SPIFFS.h"
#include <JPEGDecoder.h>
#include <ESPAsyncWebServer.h>
#include <Ticker.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BME680.h>
#include "BluetoothSerial.h"
#include "MAX30105.h"
#include "heartRate.h"
#include "Heart_Rate_Image.h"

#define EEPROM_SIZE 100 // Size in bytes
#define SSID_ADDR 0     // Start address for SSID
#define SSID_LEN 32
#define PASS_ADDR SSID_ADDR + SSID_LEN // Start address for Password
#define PASS_LEN 64

#define WIFI_MODE_SAVE_ADDR PASS_ADDR + PASS_LEN
#define MODE_SAVE_ADDR WIFI_MODE_SAVE_ADDR + 1

#define AP_MODE 0
#define STA_MODE 1

char ssid[SSID_LEN + 1] = "";
char password[PASS_LEN + 1] = "";

bool AP_OR_STA_MODE = AP_MODE;

#define BUTTON_UP 32
#define BUTTON_DOWN 33
#define TOUCH_SELECT T4 // GPIO 13
#define HAPTIC_PIN 26

#define TOUCH_INTERRUPT_THRESHOLD 20

bool rejectUpload = false;

volatile bool upPressed = false;
volatile bool downPressed = false;
volatile bool selectPressed = false;

unsigned long lastSelectTime = 0;
const unsigned long selectDebounceDelay = 350; // 350ms debounce

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 350;

unsigned long lastImageTime = 0;
const unsigned long imageInterval = 2000; // 2 seconds
int currentImageIndex = 0;

unsigned long lastBMETime = 0;
const unsigned long BMEInterval = 500;

const char *menuItems[] = {"Back", "Defualt AP", "User Mode", "Only Displ", "Weather", "Heart Rate"};
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
const int visibleItems = 5;   // Number of items visible on screen
int selectedItem = 0;
int topItem = 0;  // Index of the first visible menu item

Ticker restartTimer;

enum
{
  DEFUALT_AP_MODE,
  USER_MODE,
  DISPLAY_ONLY,
  BME_MODE,
  HEART_RATE_MONITOR
};
uint8_t MODE = DEFUALT_AP_MODE;

BluetoothSerial SerialBT;

MAX30105 particleSensor;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
int pulseAvg;
int cnt;
String preHeartBeat, HeartBeat;

AsyncWebServer server(80);

#define FORMAT_SPIFFS_IF_FAILED true

char Files[35][80];
int file_cnt = 0;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft); // Sprite object

bool pause_display = false;

enum {
  NONE,
  BME280,
  BME680
};

#define BME280_ADDRESS 0x76

uint8_t select_sensor = NONE;

Adafruit_BME280 bme280; // I2C
Adafruit_BME680 bme680; // I2C

#define TEMP_X 40
#define TEMP_Y 80
#define HUM_X 120
#define HUM_Y 80
#define RADIUS 30

float lastTemp = -1;
float lastHum = -1;

void IRAM_ATTR handleUp()
{
  if (millis() - lastSelectTime > selectDebounceDelay)
  {
    upPressed = true;
    lastSelectTime = millis();
  }
}

void IRAM_ATTR handleDown()
{
  if (millis() - lastSelectTime > selectDebounceDelay)
  {
    downPressed = true;
    lastSelectTime = millis();
  }
}

void IRAM_ATTR onTouchSelect()
{
  if (millis() - lastSelectTime > selectDebounceDelay)
  {
    selectPressed = true;
    lastSelectTime = millis();
  }
}

void setup()
{
  Serial.begin(115200);
  initSPIFFS();
  memset(Files, 0, sizeof(Files));
  listDir(SPIFFS, "/", 0);
  tft.begin();
  tft.setRotation(1);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(HAPTIC_PIN, OUTPUT);
  digitalWrite(HAPTIC_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(BUTTON_UP), handleUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN), handleDown, FALLING);
  touchAttachInterrupt(TOUCH_SELECT, onTouchSelect, TOUCH_INTERRUPT_THRESHOLD);

  retrive_credentials();
  if (MODE == BME_MODE)
  {
    BME_Init();
  }
  if ((MODE == DEFUALT_AP_MODE) || (MODE == USER_MODE))
  {
    xTaskCreatePinnedToCore(ExtraTasks, "ExtraTasks", 3500 * 3, NULL, 10, NULL, 0);
  }
  if (MODE == HEART_RATE_MONITOR)
  {
    SerialBT.begin("ESP32_BLE"); //Bluetooth device name
    InitHeartRate();
  }
}

void loop()
{
  HandleJPGImages();
  HandleMenuItems();
  delay(10);
}
