#include "FastLED.h"
#define NUM_LEDS 56
#define DATA_PIN 17

CRGB leds[NUM_LEDS];

// receive
// 1 byte header 0x17
// 1 byte length
// 1 byte overall brightness
// 1 byte checksum
// 168 bytes led brightness data
// Total: 172 bytes
const int recvSize = 172;
byte recvBuffer[recvSize];
int recvCounter = 0;

void setup() {
  Serial2.begin(1000000);

  Serial.begin(115200);
  delay(1000);
  Serial.println("Start");

  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.showColor(CHSV(50, 255, 255));
  FastLED.setCorrection(TypicalSMD5050);
  FastLED.setBrightness( 10 );
}

int loopCnt = 0;
float sliderFilt = 0.0f;
void loop() {

  while (Serial2.available() > 0) {
    byte c = Serial2.read();

    if (recvCounter == 0 ) {
      if (c == 0x17) {
        recvBuffer[recvCounter] = c;
        recvCounter++;
      }
    } else if(recvCounter == 1) {
      if(c == recvSize) {
        recvBuffer[recvCounter] = c;
        recvCounter++;
      } else {
        memset(recvBuffer, 0x00, recvSize);
        recvCounter = 0;
      }
    } else {
      recvBuffer[recvCounter] = c;
      recvCounter++;

      if(recvCounter == recvSize) {
        byte calcChecksum = 0x00;
        for(int i=0; i<recvSize; i++) {
          calcChecksum ^= recvBuffer[i];
        }

        if(calcChecksum == recvBuffer[3]) {
          Serial.println("Correct checksum");
          FastLED.setBrightness(recvBuffer[2]);
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = recvBuffer[i+4];
          }
          FastLED.show();
        }
      }
    }
  }
  
  // Turn the LED on, then pause
  CRGB current = CRGB::White;

  switch (loopCnt % 6) {
    case 0:
      current = CRGB::Red;
      break;

    case 1:
      current = CRGB::Green;
      break;

    case 2:
      current = CRGB::Blue;
      break;

    case 3:
      current = CRGB::Yellow;
      break;

    case 4:
      current = CRGB::Magenta;
      break;

    case 5:
      current = CRGB::Cyan;
      break;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = current;
  }

  //leds[12] = CRGB::White;
  FastLED.show();

  delay(500);
  // Now turn the LED off, then pause
  for (int i = 0; i < 12; i++) {
    leds[i] = CRGB::Black;
  }

  //leds[12] = CRGB::Black;
  FastLED.show();
  delay(500);

  Serial.println("Loop: " + String(loopCnt));
  loopCnt++;
}


