/*
   Based on design and code by Andy Doro:
   https://github.com/andydoro/WordClock-NeoMatrix8x8

*/


#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <SmartLeds.h>
#include "displayTime.h"

const char* ssid       = "yourssid";
const char* password   = "yourpassw0rd";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

#define LED_PIN     32

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  64
const int CHANNEL = 0;
double brightness = 0.50;

SmartLed leds( LED_WS2812B, LED_COUNT, LED_PIN, CHANNEL, DoubleBuffer );

Rgb bootColors[8] = {Rgb(0x2C, 0x00, 0xFF),
                     Rgb(0xFF, 0x00, 0xE8), 
                     Rgb(0xFF, 0x00, 0x00),
                     Rgb(0xFF, 0x8A, 0x00), 
                     Rgb(0xFF, 0xF0, 0x00), 
                     Rgb(0x04, 0xFF, 0x00), 
                     Rgb(0x00, 0xFF, 0xEE),
                     Rgb(0xFF, 0x00, 0x00)};

struct tm timeInfo;
int counter = 0;

void printLocalTime() {

  timeInfo.tm_hour = counter / 60;
  timeInfo.tm_min = counter % 60;

  counter++;
  Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
  getWordMask(&leds, &timeInfo);
  leds.show();
  leds.wait();
}



void drawBootSequence(uint8_t i) {
  if (i > 8) i = 8;
  for (int y = 0; y <= i; y++) {
    for (int x = 0; x < 8 - y; x++) {
      Hsv hsv = Hsv(bootColors[y]);
      hsv.v = hsv.v * brightness;
      leds[ getLedIndex(7 - x, y) ] = hsv;
    }

  }
  leds.show();
  leds.wait();
  delay(250);

}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
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
  for (uint8_t i = counter; i < 8; i++) {
    drawBootSequence(i);
  }
  Serial.println("Connected");

  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);


}

void loop()
{
  delay(200);
  printLocalTime();
}