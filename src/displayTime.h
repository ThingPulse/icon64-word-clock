#include <Arduino.h>
#include <time.h>

#define MFIVE    mask |= 0xF00000000000;log_d("MFIVE");        // these are in hexadecimal
#define MTEN     mask |= 0x5800000000000000;log_d("MTEN"); 
#define AQUARTER mask |= 0x80FE000000000000;log_d("AQUARTER"); 
#define TWENTY   mask |= 0x7E00000000000000;log_d("TWENTY"); 
#define HALF     mask |= 0xF0000000000;log_d("HALF"); 
#define PAST     mask |= 0x7800000000;log_d("PAST"); 
#define TO       mask |= 0xC00000000;log_d("TO"); 
#define ONE      mask |= 0x43;log_d("ONE"); 
#define TWO      mask |= 0xC040;log_d("TWO"); 
#define THREE    mask |= 0x1F0000;log_d("THREE"); 
#define FOUR     mask |= 0xF0;log_d("FOUR"); 
#define FIVE     mask |= 0xF0000000;log_d("FIVE"); 
#define SIX      mask |= 0xE00000;log_d("SIX"); 
#define SEVEN    mask |= 0x800F00;log_d("SEVEN"); 
#define EIGHT    mask |= 0x1F000000;log_d("EIGHT"); 
#define NINE     mask |= 0xF;log_d("NINE"); 
#define TEN      mask |= 0x1010100;log_d("TEN"); 
#define ELEVEN   mask |= 0x3F00;log_d("ELEVEN"); 
#define TWELVE   mask |= 0xF600;log_d("TWELVE"); 
#define ANDYDORO mask |= 0x8901008700000000;log_d("ANDYDORO"); 
// function to generate the right "phrase" based on the time

uint64_t getWordMask(struct tm * theTime) {

  // get time from the RTC
  //DateTime theTime = RTC.now();
  //theTime = calculateTime(); // takes into account DST

  // serial print current time
  /*
  Serial.print(theTime.year(), DEC);
  Serial.print('/');
  Serial.print(theTime.month(), DEC);
  Serial.print('/');
  Serial.print(theTime.day(), DEC);
  Serial.print(' ');
  Serial.print(theTime->tm_hour, DEC);
  Serial.print(':');
  Serial.print(theTime->tm_min, DEC);
  Serial.print(':');
  Serial.print(theTime.second(), DEC);
  Serial.println();
  */
  uint64_t mask = 0;

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








