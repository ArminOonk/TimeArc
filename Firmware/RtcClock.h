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

  void setTimeZone(int tz);
  int getTimeZone();

  void setDayLightSaving(bool dls);
  bool getDayLightSaving();

  private:
  void RTC_Configuration(void);

  const static unsigned int magicNumber = 0xA5A5;
  int timeZone;
  bool dayLightSaving;
};

#endif