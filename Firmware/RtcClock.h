#ifndef RTC_CLOCK
#define RTC_CLOCK

#include <time.h>

class rtcClock
{
  public:
  rtcClock();
  bool init();

  void setTime(time_t t);
  time_t getTime();
};

#endif