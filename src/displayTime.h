// based on / inspired by https://github.com/andydoro/WordClock-NeoMatrix8x8/blob/master/WordClock_NeoMatrix8x8/displayTime.ino

#include <Arduino.h>
#include <time.h>
#include <FastLED.h>

#define MFIVE    displayWord(leds, 0xF00000000000, CRGB(0x2C, 0x00, 0xFF))        // these are in hexadecimal
#define MTEN     displayWord(leds, 0x5800000000000000, CRGB(0x2C, 0x00, 0xFF));
#define AQUARTER displayWord(leds, 0x80FE000000000000, CRGB(0x2C, 0x00, 0xFF));
#define TWENTY   displayWord(leds, 0x7E00000000000000, CRGB(0x2C, 0x00, 0xFF));
#define HALF     displayWord(leds, 0xF0000000000, CRGB(0x2C, 0x00, 0xFF));
#define PAST     displayWord(leds, 0x7800000000, CRGB(0xFF, 0x00, 0x00));
#define TO       displayWord(leds, 0xC00000000, CRGB(0xFF, 0x00, 0x00));
#define ONE      displayWord(leds, 0x43, CRGB(0x00, 0xFF, 0xEE));
#define TWO      displayWord(leds, 0xC040, CRGB(0x00, 0xFF, 0xEE));
#define THREE    displayWord(leds, 0x1F0000 , CRGB(0x00, 0xFF, 0xEE));
#define FOUR     displayWord(leds, 0xF0, CRGB(0x00, 0xFF, 0xEE));
#define FIVE     displayWord(leds, 0xF0000000, CRGB(0x00, 0xFF, 0xEE));
#define SIX      displayWord(leds, 0xE00000, CRGB(0x00, 0xFF, 0xEE));
#define SEVEN    displayWord(leds, 0x800F00, CRGB(0x00, 0xFF, 0xEE));
#define EIGHT    displayWord(leds, 0x1F000000, CRGB(0x00, 0xFF, 0xEE));
#define NINE     displayWord(leds, 0xF, CRGB(0x00, 0xFF, 0xEE));
#define TEN      displayWord(leds, 0x1010100, CRGB(0x00, 0xFF, 0xEE));
#define ELEVEN   displayWord(leds, 0x3F00, CRGB(0x00, 0xFF, 0xEE));
#define TWELVE   displayWord(leds, 0xF600, CRGB(0x00, 0xFF, 0xEE));


uint8_t getLedIndex(uint8_t x, uint8_t y) {
  //x = 7 - x;
  if (y % 2 == 0) {
    return y * 8 + x;
  } else {
    return y * 8 + (7 - x);
  }
}

void displayWord(CRGB *leds, uint64_t mask, CRGB color) {
  uint8_t bitIndex = 0;
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {

      boolean lastBit = bitRead(mask, bitIndex);
      CHSV hsv = rgb2hsv_approximate(color);
      if (lastBit) {
        hsv.h = hsv.h + millis() / 200 + bitIndex;
        leds[getLedIndex(7 - x, y)] = hsv;
      }

      bitIndex++;
    }
  }
}

// function to generate the right "phrase" based on the time

uint64_t getWordMask(CRGB *leds, struct tm * theTime) {

  uint64_t mask = 0;

  for (int i = 0; i < 64; i++) {
      leds->operator [] (i) = CRGB::Black;
  }

  // time we display the appropriate theTime->tm_min counter
  if ((theTime->tm_min > 4) && (theTime->tm_min < 10)) {
    MFIVE;
    //Serial.print("five");
  }
  if ((theTime->tm_min > 9) && (theTime->tm_min < 15)) {
    MTEN;
    //Serial.print("ten");
  }
  if ((theTime->tm_min > 14) && (theTime->tm_min < 20)) {
    AQUARTER;
    //Serial.print("a quarter");
  }
  if ((theTime->tm_min > 19) && (theTime->tm_min < 25)) {
    TWENTY;
    //Serial.print("twenty");
  }
  if ((theTime->tm_min > 24) && (theTime->tm_min < 30)) {
    TWENTY;
    MFIVE;
    //Serial.print("twenty five");
  }
  if ((theTime->tm_min > 29) && (theTime->tm_min < 35)) {
    HALF;
    //Serial.print("half");
  }
  if ((theTime->tm_min > 34) && (theTime->tm_min < 40)) {
    TWENTY;
    MFIVE;
    //Serial.print("twenty five");
  }
  if ((theTime->tm_min > 39) && (theTime->tm_min < 45)) {
    TWENTY;
    //Serial.print("twenty");
  }
  if ((theTime->tm_min > 44) && (theTime->tm_min < 50)) {
    AQUARTER;
    //Serial.print("a quarter");
  }
  if ((theTime->tm_min > 49) && (theTime->tm_min < 55)) {
    MTEN;
    //Serial.print("ten");
  }
  if (theTime->tm_min > 54) {
    MFIVE;
    //Serial.print("five");
  }

  if ((theTime->tm_min < 5))
  {
    switch (theTime->tm_hour) {
      case 1:
      case 13:
        ONE;
        break;
      case 2:
      case 14:
        TWO;
        break;
      case 3:
      case 15:
        THREE;
        break;
      case 4:
      case 16:
        FOUR;
        break;
      case 5:
      case 17:
        FIVE;
        break;
      case 6:
      case 18:
        SIX;
        break;
      case 7:
      case 19:
        SEVEN;
        break;
      case 8:
      case 20:
        EIGHT;
        break;
      case 9:
      case 21:
        NINE;
        break;
      case 10:
      case 22:
        TEN;
        break;
      case 11:
      case 23:
        ELEVEN;
        break;
      case 0:
      case 12:
        TWELVE;
        break;
    }

  }
  else if ((theTime->tm_min < 35) && (theTime->tm_min > 4))
  {
    PAST;
    //Serial.print(" past ");
    switch (theTime->tm_hour) {
      case 1:
      case 13:
        ONE;
        break;
      case 2:
      case 14:
        TWO;
        break;
      case 3:
      case 15:
        THREE;
        break;
      case 4:
      case 16:
        FOUR;
        break;
      case 5:
      case 17:
        FIVE;
        break;
      case 6:
      case 18:
        SIX;
        break;
      case 7:
      case 19:
        SEVEN;
        break;
      case 8:
      case 20:
        EIGHT;
        break;
      case 9:
      case 21:
        NINE;
        break;
      case 10:
      case 22:
        TEN;
        break;
      case 11:
      case 23:
        ELEVEN;
        break;
      case 0:
      case 12:
        TWELVE;
        break;
    }
  }
  else
  {
    // if we are greater than 34 minutes past the hour then display
    // the next hour, as we will be displaying a 'to' sign
    TO;
    //Serial.print(" to ");
    switch (theTime->tm_hour) {
      case 1:
      case 13:
        TWO;
        break;
      case 14:
      case 2:
        THREE;
        break;
      case 15:
      case 3:
        FOUR;
        break;
      case 4:
      case 16:
        FIVE;
        break;
      case 5:
      case 17:
        SIX;
        break;
      case 6:
      case 18:
        SEVEN;
        break;
      case 7:
      case 19:
        EIGHT;
        break;
      case 8:
      case 20:
        NINE;
        break;
      case 9:
      case 21:
        TEN;
        break;
      case 10:
      case 22:
        ELEVEN;
        break;
      case 11:
      case 23:
        TWELVE;
        break;
      case 0:
      case 12:
        ONE;
        break;
    }
  }
  return mask;
  //applyMask(); // apply phrase mask to colorshift function
}
