/*
MIT License

Copyright (c) 2020 ThingPulse GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
   Based on design and code by Andy Doro:
   https://github.com/andydoro/WordClock-NeoMatrix8x8
*/

#include <Arduino.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include "time.h"
#include <FastLED.h>
#include "displayTime.h"
#include "TZ.h"
#include "TZinfo.h"


// ********* user settings *********
String ssid = "yourssid";
String password = "yourpassw0rd";
// If true the colors change every second thereby mimicking the second counter
// of a regular watch. If false the colors transition gradually and therefore
// smoothly.
bool secondCounterEffect = true;
// "TZ_" macros follow DST change across seasons without source code change
// check for your nearest city in src/TZ.h
String timezone = TZ_Europe_Zurich;
const char *ntpServer = "pool.ntp.org";
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
void displayLocalTime();
void drawBootSequence(uint8_t i);
void initTime();
void loadPropertiesFromSpiffs();
// ********* END forward declarations *********


void setup() {
  Serial.begin(115200);
  delay(1000);
  loadPropertiesFromSpiffs();

  if (secondCounterEffect) {
    loopDelay = 1000;
  } else {
    loopDelay = 50;
  }

  FastLED.addLeds<WS2812B, LED_PIN>(leds, LED_COUNT);
  FastLED.setBrightness(brightness * 255.0);
  // connect to WiFi
  Serial.printf("Connecting to %s ", ssid.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());
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
  configTzTime(timezone.c_str(), ntpServer);

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

void loadPropertiesFromSpiffs() {
  if (SPIFFS.begin()) {
    Serial.println("Attempting to read application.properties file from SPIFFS.");
    File f = SPIFFS.open("/application.properties");
    if (f) {
      Serial.println("File exists. Reading and assigning properties.");
      while (f.available()) {
        String key = f.readStringUntil('=');
        String value = f.readStringUntil('\n');
        if (key == "ssid") {
          ssid = value.c_str();
          Serial.println("Using 'ssid' from SPIFFS");
        } else if (key == "password") {
          password = value.c_str();
          Serial.println("Using 'password' from SPIFFS");
        } else if (key == "timezone") {
          timezone = getTzInfo(value.c_str());
          Serial.println("Using 'timezone' from SPIFFS");
        } else if (key == "secondCounterEffect") {
          secondCounterEffect = value == "true" ? true : false;
          Serial.println("Using 'secondCounterEffect' from SPIFFS");
        }
      }
    }
    f.close();
    Serial.println("Effective properties now as follows:");
    Serial.println("\tssid: " + ssid);
    Serial.println("\tpassword: " + password);
    Serial.println("\timezone: " + timezone);
    Serial.println("\tsecond counter effect: " + String(secondCounterEffect ? "true" : "false"));
  } else {
    Serial.println("SPIFFS mount failed.");
  }
}
