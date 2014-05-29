#ifndef DISPLAY_BUFFER
#define DISPLAY_BUFFER
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "defines.h"
#include <string.h>
#include <time.h>


const int totalNrLeds = 144;
const int displayBufferSize = 144/8;

// DOZEN pins
#define DOZEN1_PORT   GPIOB
#define DOZEN1_PIN    GPIO_Pin_10
#define DOZEN1_LOW     DOZEN1_PORT->BRR |= DOZEN1_PIN
#define DOZEN1_HIGH    DOZEN1_PORT->BSRR |= DOZEN1_PIN

#define DOZEN2_PORT   GPIOB
#define DOZEN2_PIN    GPIO_Pin_11
#define DOZEN2_LOW     DOZEN2_PORT->BRR |= DOZEN2_PIN
#define DOZEN2_HIGH    DOZEN2_PORT->BSRR |= DOZEN2_PIN

#define DOZEN3_PORT   GPIOB
#define DOZEN3_PIN    GPIO_Pin_12
#define DOZEN3_LOW     DOZEN3_PORT->BRR |= DOZEN3_PIN
#define DOZEN3_HIGH    DOZEN3_PORT->BSRR |= DOZEN3_PIN

#define DOZEN4_PORT   GPIOB
#define DOZEN4_PIN    GPIO_Pin_13
#define DOZEN4_LOW     DOZEN4_PORT->BRR |= DOZEN4_PIN
#define DOZEN4_HIGH    DOZEN4_PORT->BSRR |= DOZEN4_PIN

#define DOZEN5_PORT   GPIOB
#define DOZEN5_PIN    GPIO_Pin_14
#define DOZEN5_LOW     DOZEN5_PORT->BRR |= DOZEN5_PIN
#define DOZEN5_HIGH    DOZEN5_PORT->BSRR |= DOZEN5_PIN

#define DOZEN6_PORT   GPIOB
#define DOZEN6_PIN    GPIO_Pin_15
#define DOZEN6_LOW     DOZEN6_PORT->BRR |= DOZEN6_PIN
#define DOZEN6_HIGH    DOZEN6_PORT->BSRR |= DOZEN6_PIN

#define DOZEN7_PORT   GPIOC
#define DOZEN7_PIN    GPIO_Pin_9
#define DOZEN7_LOW     DOZEN7_PORT->BRR |= DOZEN7_PIN
#define DOZEN7_HIGH    DOZEN7_PORT->BSRR |= DOZEN7_PIN

#define DOZEN8_PORT   GPIOC
#define DOZEN8_PIN    GPIO_Pin_8
#define DOZEN8_LOW     DOZEN8_PORT->BRR |= DOZEN8_PIN
#define DOZEN8_HIGH    DOZEN8_PORT->BSRR |= DOZEN8_PIN

#define DOZEN9_PORT   GPIOC
#define DOZEN9_PIN    GPIO_Pin_12
#define DOZEN9_LOW     DOZEN9_PORT->BRR |= DOZEN9_PIN
#define DOZEN9_HIGH    DOZEN9_PORT->BSRR |= DOZEN9_PIN

#define DOZEN10_PORT   GPIOC
#define DOZEN10_PIN    GPIO_Pin_11
#define DOZEN10_LOW     DOZEN10_PORT->BRR |= DOZEN10_PIN
#define DOZEN10_HIGH    DOZEN10_PORT->BSRR |= DOZEN10_PIN

#define DOZEN11_PORT   GPIOC
#define DOZEN11_PIN    GPIO_Pin_10
#define DOZEN11_LOW     DOZEN11_PORT->BRR |= DOZEN11_PIN
#define DOZEN11_HIGH    DOZEN11_PORT->BSRR |= DOZEN11_PIN

#define DOZEN12_PORT   GPIOC
#define DOZEN12_PIN    GPIO_Pin_5
#define DOZEN12_LOW     DOZEN12_PORT->BRR |= DOZEN12_PIN
#define DOZEN12_HIGH    DOZEN12_PORT->BSRR |= DOZEN12_PIN

// ARCDOZEN
#define ARCDOZEN1_PORT   GPIOB
#define ARCDOZEN1_PIN    GPIO_Pin_1
#define ARCDOZEN1_LOW     ARCDOZEN1_PORT->BRR |= ARCDOZEN1_PIN
#define ARCDOZEN1_HIGH    ARCDOZEN1_PORT->BSRR |= ARCDOZEN1_PIN

#define ARCDOZEN2_PORT   GPIOB
#define ARCDOZEN2_PIN    GPIO_Pin_0
#define ARCDOZEN2_LOW     ARCDOZEN2_PORT->BRR |= ARCDOZEN2_PIN
#define ARCDOZEN2_HIGH    ARCDOZEN2_PORT->BSRR |= ARCDOZEN2_PIN

#define ARCDOZEN3_PORT   GPIOA
#define ARCDOZEN3_PIN    GPIO_Pin_3
#define ARCDOZEN3_LOW     ARCDOZEN3_PORT->BRR |= ARCDOZEN3_PIN
#define ARCDOZEN3_HIGH    ARCDOZEN3_PORT->BSRR |= ARCDOZEN3_PIN

#define ARCDOZEN4_PORT   GPIOA
#define ARCDOZEN4_PIN    GPIO_Pin_2
#define ARCDOZEN4_LOW     ARCDOZEN4_PORT->BRR |= ARCDOZEN4_PIN
#define ARCDOZEN4_HIGH    ARCDOZEN4_PORT->BSRR |= ARCDOZEN4_PIN

#define ARCDOZEN5_PORT   GPIOA
#define ARCDOZEN5_PIN    GPIO_Pin_1
#define ARCDOZEN5_LOW     ARCDOZEN5_PORT->BRR |= ARCDOZEN5_PIN
#define ARCDOZEN5_HIGH    ARCDOZEN5_PORT->BSRR |= ARCDOZEN5_PIN

#define ARCDOZEN6_PORT   GPIOA
#define ARCDOZEN6_PIN    GPIO_Pin_0
#define ARCDOZEN6_LOW     ARCDOZEN6_PORT->BRR |= ARCDOZEN6_PIN
#define ARCDOZEN6_HIGH    ARCDOZEN6_PORT->BSRR |= ARCDOZEN6_PIN

#define ARCDOZEN7_PORT   GPIOB
#define ARCDOZEN7_PIN    GPIO_Pin_9
#define ARCDOZEN7_LOW     ARCDOZEN7_PORT->BRR |= ARCDOZEN7_PIN
#define ARCDOZEN7_HIGH    ARCDOZEN7_PORT->BSRR |= ARCDOZEN7_PIN

#define ARCDOZEN8_PORT   GPIOB
#define ARCDOZEN8_PIN    GPIO_Pin_8
#define ARCDOZEN8_LOW     ARCDOZEN8_PORT->BRR |= ARCDOZEN8_PIN
#define ARCDOZEN8_HIGH    ARCDOZEN8_PORT->BSRR |= ARCDOZEN8_PIN

#define ARCDOZEN9_PORT   GPIOB
#define ARCDOZEN9_PIN    GPIO_Pin_7
#define ARCDOZEN9_LOW     ARCDOZEN9_PORT->BRR |= ARCDOZEN9_PIN
#define ARCDOZEN9_HIGH    ARCDOZEN9_PORT->BSRR |= ARCDOZEN9_PIN

#define ARCDOZEN10_PORT   GPIOB
#define ARCDOZEN10_PIN    GPIO_Pin_6
#define ARCDOZEN10_LOW     ARCDOZEN10_PORT->BRR |= ARCDOZEN10_PIN
#define ARCDOZEN10_HIGH    ARCDOZEN10_PORT->BSRR |= ARCDOZEN10_PIN

#define ARCDOZEN11_PORT   GPIOC
#define ARCDOZEN11_PIN    GPIO_Pin_7
#define ARCDOZEN11_LOW     ARCDOZEN11_PORT->BRR |= ARCDOZEN11_PIN
#define ARCDOZEN11_HIGH    ARCDOZEN11_PORT->BSRR |= ARCDOZEN11_PIN

#define ARCDOZEN12_PORT   GPIOC
#define ARCDOZEN12_PIN    GPIO_Pin_6
#define ARCDOZEN12_LOW     ARCDOZEN12_PORT->BRR |= ARCDOZEN12_PIN
#define ARCDOZEN12_HIGH    ARCDOZEN12_PORT->BSRR |= ARCDOZEN12_PIN


enum clockIndex_t
{
  SECOND,
  MINUTE,
  HOUR,
};

class displayBuffer
{
  public:
  displayBuffer();

  void switchBuffer();
  void run();

  void ledOn(int ledNr);
  void ledOff(int ledNr);

  void setTime(time_t now);
  void hourOn(unsigned int hour);
  void minuteOn(unsigned int minute);
  void secondOn(unsigned int second);

  void setIntensity(float intensity);
  void displayOff();

  private:
  void switchLedOn(int ledNr);
  
  void clearBuffer(int bufferNr);
  int nextBufferNr();
  
  bool isLedOn(int ledNr);
  char* getShowBuffer();
  char* getPaintBuffer();

  void animationMode();
  void clockMode();

  static const int nrBuffers = 2;
  int currentBuffer;
  char buffer[nrBuffers][displayBufferSize];
  int runCnt;

  int clockLedNr[3];
  int timerPeriod;

};

#endif