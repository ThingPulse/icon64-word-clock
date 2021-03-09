/*
   Based on design and code by Andy Doro:
   https://github.com/andydoro/WordClock-NeoMatrix8x8
*/

#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <FastLED.h>
#include "displayTime.h"
#include "TZ.h"


// ********* user settings *********
const char* ssid      = "yourssid";
const char* password  = "yourpassw0rd";
const char *ntpServer = "pool.ntp.org";
// If true the colors change every second thereby mimicking the second counter
// of a regular watch. If false the colors transition gradually and therefore
// smoothly.
const bool secondCounterEffect = true;
// "TZ_" macros follow DST change across seasons without source code change
// check for your nearest city in src/TZ.h
#define MYTZ TZ_Europe_Zurich
// ********* END user settings *********


#define LED_PIN 32
#define LED_COUNT 64
const int CHANNEL = 0;
double brightness = 0.40;
int loopDelay;

CRGB bootColors[8] = {CRGB(0x2C, 0x00, 0xFF),
                     CRGB(0xFF, 0x00, 0xE8),
                     CRGB(0xFF, 0x00, 0x00),
                     CRGB(0xFF, 0x8A, 0x00),
                     CRGB(0xFF, 0xF0, 0x00),
                     CRGB(0x04, 0xFF, 0x00),
                     CRGB(0x00, 0xFF, 0xEE),
                     CRGB(0xFF, 0x00, 0x00)};

CRGB leds[LED_COUNT];

struct tm timeInfo;


// ********* forward declarations *********
void drawBootSequence(uint8_t i);
void initTime();
void displayLocalTime();
// ********* END forward declarations *********


void setup() {
  Serial.begin(115200);

  if (secondCounterEffect) {
    loopDelay = 1000;
  } else {
    loopDelay = 50;
  }

  FastLED.addLeds<WS2812B, LED_PIN>(leds, LED_COUNT);
  FastLED.setBrightness(brightness * 255.0);
  // connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  drawBootSequence(1);
  // Try forever
  int counter = 2;
  while (WiFi.status() != WL_CONNECTED) {
    drawBootSequence(counter);
    counter++;
    Serial.println("...Connecting to WiFi");
  }
  FastLED.clear();
  for (uint8_t i = counter; i < 8; i++) {
    drawBootSequence(i);
  }
  Serial.println("Connected");

  initTime();
  displayLocalTime();
}

void loop() {
  delay(loopDelay);
  displayLocalTime();
}


void drawBootSequence(uint8_t i) {
  if (i > 8) {
    i = 8;
  }
  FastLED.clear();
  for (int y = 0; y <= i; y++) {
    for (int x = 0; x < 8 - y; x++) {
      CHSV hsv = rgb2hsv_approximate(bootColors[y]);
      leds[getLedIndex(7 - x, y)] = hsv;
    }

  }
  FastLED.show();
  delay(500);
}

void initTime() {
  configTzTime(MYTZ, ntpServer);

  // wait for NTP to properly sync
  int i = 0;
  while (time(nullptr) < 1615192357 && i < 40) {
    Serial.print(".");
    delay(500);
    i++;
    yield();
  }
  Serial.println();
}

void displayLocalTime() {
  FastLED.clear();
  if(!getLocalTime(&timeInfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
  getWordMask(leds, &timeInfo);
  FastLED.show();
}
