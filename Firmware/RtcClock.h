#ifndef RTC_CLOCK
#define RTC_CLOCK

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <time.h>
#include <stdio.h>

class rtcClock
{
  public:
  rtcClock();
  bool init();

  
  void setTime(time_t t);
  time_t getTime();
  time_t getUTC();

  void setOffset(int offset);
  int getOffset();

  void printOffset();
  void printTime();

  private:
  void RTC_Configuration(void);

  const static unsigned int magicNumber = 0xA5A5;
  int offset;
  bool dayLightSaving;
};

#endif