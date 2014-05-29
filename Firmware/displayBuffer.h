#ifndef DISPLAY_BUFFER
#define DISPLAY_BUFFER
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "defines.h"
#include <string.h>
#include <time.h>

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

  private:
  void switchLedOn(int ledNr);
  void displayOff();

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
};

#endif