#include "displayBuffer.h"

displayBuffer::displayBuffer()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_StructInit(&GPIO_InitStructure);

  // DOZEN Pins
  GPIO_InitStructure.GPIO_Pin   = DOZEN1_PIN | DOZEN2_PIN | DOZEN3_PIN | DOZEN4_PIN | DOZEN5_PIN | DOZEN6_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DOZEN1_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = DOZEN7_PIN | DOZEN8_PIN | DOZEN9_PIN | DOZEN10_PIN | DOZEN11_PIN | DOZEN12_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DOZEN7_PORT, &GPIO_InitStructure);

  // ARC DOZEN (PWM timer)
  GPIO_InitStructure.GPIO_Pin   = ARCDOZEN1_PIN | ARCDOZEN2_PIN | ARCDOZEN7_PIN | ARCDOZEN8_PIN | ARCDOZEN9_PIN | ARCDOZEN10_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ARCDOZEN1_PORT, &GPIO_InitStructure);

  // ARC DOZEN (PWM timer)
  GPIO_InitStructure.GPIO_Pin   = ARCDOZEN3_PIN | ARCDOZEN4_PIN | ARCDOZEN5_PIN | ARCDOZEN6_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ARCDOZEN3_PORT, &GPIO_InitStructure);
  
  // ARC DOZEN (PWM timer)
  GPIO_InitStructure.GPIO_Pin   = ARCDOZEN11_PIN | ARCDOZEN12_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ARCDOZEN11_PORT, &GPIO_InitStructure);

  displayOff();

  currentBuffer = 0;
  for(int i=0; i<nrBuffers; i++)
  {
    clearBuffer(i);
  }

  runCnt = 0;

  clockLedNr[SECOND] = 0;
  clockLedNr[MINUTE] = 0;
  clockLedNr[HOUR] = 0;
}

void displayBuffer::setTime(time_t now)
{
  struct tm * ptm;
  ptm = gmtime ( &now );

  hourOn(ptm->tm_hour%12);
  minuteOn(ptm->tm_min);
  secondOn(ptm->tm_sec);
  switchBuffer();
}

void displayBuffer::hourOn(unsigned int hour)
{
  if(hour < 12)
  {
    clockLedNr[HOUR] = ((hour+6)*12)%totalNrLeds;
    ledOn(clockLedNr[HOUR]);
  }
}

void displayBuffer::minuteOn(unsigned int minute)
{
  if(minute < 60)
  {
    int div = minute/5;
    int remainder = minute%5;

    clockLedNr[MINUTE] = ((div*12+remainder+1) + (totalNrLeds/2))%totalNrLeds;
    ledOn(clockLedNr[MINUTE]);
  }
}

void displayBuffer::secondOn(unsigned int second)
{
  if(second < 60)
  {
    int div = second/5;
    int remainder = second%5;

    clockLedNr[SECOND] = ((div*12+remainder+6) + (totalNrLeds/2))%totalNrLeds;
    ledOn(clockLedNr[SECOND]);
  }
}

void displayBuffer::switchBuffer()
{
  int newPaintBuffer = currentBuffer;
  currentBuffer = nextBufferNr();
  clearBuffer(newPaintBuffer);
}

bool displayBuffer::isLedOn(int ledNr)
{
  char *buf = getShowBuffer();
  char index = buf[ledNr/8];
  char bitpos = (1<<(ledNr%8));

  return ( (index & bitpos) != 0);
}

void displayBuffer::run()
{
  displayOff();
  //animationMode();
  clockMode();
}

void displayBuffer::ledOn(int ledNr)
{
  char *buf = getPaintBuffer();
  buf[ledNr/8] |= (1<<(ledNr%8));
}

void displayBuffer::ledOff(int ledNr)
{
  char *buf = getPaintBuffer();
  buf[ledNr/8] &= ~(1<<(ledNr%8));
}

/// Private
void displayBuffer::switchLedOn(int ledNr)
{
  switch(ledNr/12)
  {
    case 0:
    ARCDOZEN1_LOW;
    break;
  
    case 1:
    ARCDOZEN2_LOW;
    break;

    case 2:
    ARCDOZEN3_LOW;
    break;

    case 3:
    ARCDOZEN4_LOW;
    break;
  
    case 4:
    ARCDOZEN5_LOW;
    break;

    case 5:
    ARCDOZEN6_LOW;
    break;

    case 6:
    ARCDOZEN7_LOW;
    break;

    case 7:
    ARCDOZEN8_LOW;
    break;

    case 8:
    ARCDOZEN9_LOW;
    break;

    case 9:
    ARCDOZEN10_LOW;
    break;

    case 10:
    ARCDOZEN11_LOW;
    break;

    case 11:
    ARCDOZEN12_LOW;
    break;
  }

  switch(ledNr%12)
  {
    case 0:
    DOZEN1_HIGH;
    break;

    case 1:
    DOZEN2_HIGH;
    break;

    case 2:
    DOZEN3_HIGH;
    break;

    case 3:
    DOZEN4_HIGH;
    break;

    case 4:
    DOZEN5_HIGH;
    break;

    case 5:
    DOZEN6_HIGH;
    break;

    case 6:
    DOZEN7_HIGH;
    break;

    case 7:
    DOZEN8_HIGH;
    break;

    case 8:
    DOZEN9_HIGH;
    break;

    case 9:
    DOZEN10_HIGH;
    break;

    case 10:
    DOZEN11_HIGH;
    break;

    case 11:
    DOZEN12_HIGH;
    break;
  }
}

void displayBuffer::displayOff()
{
  DOZEN1_LOW;
  DOZEN2_LOW;
  DOZEN3_LOW;
  DOZEN4_LOW;
  DOZEN5_LOW;
  DOZEN6_LOW;
  DOZEN7_LOW;
  DOZEN8_LOW;
  DOZEN9_LOW;
  DOZEN10_LOW;
  DOZEN11_LOW;
  DOZEN12_LOW;


  // ARCDOZEN high leds off
  ARCDOZEN1_HIGH;
  ARCDOZEN2_HIGH;
  ARCDOZEN3_HIGH;
  ARCDOZEN4_HIGH;
  ARCDOZEN5_HIGH;
  ARCDOZEN6_HIGH;
  ARCDOZEN7_HIGH;
  ARCDOZEN8_HIGH;
  ARCDOZEN9_HIGH;
  ARCDOZEN10_HIGH;
  ARCDOZEN11_HIGH;
  ARCDOZEN12_HIGH;
}

char* displayBuffer::getShowBuffer()
{
  return buffer[currentBuffer];
}

char* displayBuffer::getPaintBuffer()
{
  return buffer[nextBufferNr()];
}

int displayBuffer::nextBufferNr()
{
  int ret = currentBuffer + 1;
  if(ret >= nrBuffers)
  {
    ret = 0;
  }
  return ret;
}

void displayBuffer::clearBuffer(int bufferNr)
{
  if(bufferNr < nrBuffers)
  {
    memset(buffer[bufferNr], 0x00, displayBufferSize);
  }
}

// animationMode: all led can be on 'simultaneous' but leds will be low intensity
void displayBuffer::animationMode()
{
  if(isLedOn(runCnt))
  {
    switchLedOn(runCnt);
  }

  runCnt++;
  if(runCnt >= totalNrLeds)
  {
    runCnt = 0;
  }
}

// CLockMode only 1 second, 1 minute and 1 hour led are active
void displayBuffer::clockMode()
{
  switch(runCnt)
  {
    case 0:
    switchLedOn(clockLedNr[SECOND]);
    break;

    case 1:
    switchLedOn(clockLedNr[MINUTE]);
    break;

    case 2:
    switchLedOn(clockLedNr[HOUR]);
    break;
  }

  runCnt++;
  if(runCnt > 2)
  {
    runCnt = 0;
  }
}