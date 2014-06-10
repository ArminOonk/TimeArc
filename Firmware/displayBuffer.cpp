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

  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  //TIMER2 is control loop timer
  //Timer2 loopt op 72Mhz
  //prescaler = 180; -> timer freq is 400kHz
  //period = 397 -> interrupt freq is 999.5 Hz 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseStructure.TIM_Prescaler         = 20;
  TIM_TimeBaseStructure.TIM_Period            = 397;
  timerPeriod = TIM_TimeBaseStructure.TIM_Period;
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse =  (uint32_t)(TIM_TimeBaseStructure.TIM_Period); // Duty cycle 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

  NVIC_InitStructure.NVIC_IRQChannel                    = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM2, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);

  displayOff();

  currentBuffer = 0;
  for(int i=0; i<nrBuffers; i++)
  {
    clearBuffer(i);
  }

  runCnt = 0;

  clockTime[SECOND] = 0;
  clockTime[MINUTE] = 0;
  clockTime[HOUR] = 0;

  pose = UNKNOWN;
  mode = OFF;
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
    clockTime[HOUR] = hour;
    ledOn(getHourLedNr(clockTime[HOUR]));
  }
}

void displayBuffer::minuteOn(unsigned int minute)
{
  if(minute < 60)
  {
    clockTime[MINUTE] = minute;
    ledOn(getMinuteLedNr(clockTime[MINUTE]));
  }
}

void displayBuffer::secondOn(unsigned int second)
{
  if(second < 60)
  {
    clockTime[SECOND] = second;
    ledOn(getSecondLedNr(clockTime[SECOND]));
  }
}

void displayBuffer::setIntensity(float intensity)
{
  int intval = intensity*timerPeriod-6; // -6 to make sure intensity of 1.0 is actually max power (compensate for time during cc1 intterrupt)
  if(intval >= timerPeriod)
  {
    intval = timerPeriod - 1;
  }

  if(intval < 0)
  {
    intval = 0;
  }
  TIM_SetCompare1(TIM2, intval);
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

  switch(mode)
  {
    case OFF:
    break;

    case CLOCK:
    clockMode();
    break;

    case ANIMATION:
    animationMode();
    break;    
  }
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

void displayBuffer::setPose(pose_t p)
{
  pose = p;
}

void displayBuffer::setMode(displayMode_t m)
{
  bool print = (m != mode);
  mode = m;
  if(print)
  {
    printMode();
  }
}

displayMode_t displayBuffer::getMode()
{
  return mode;
}

void displayBuffer::printMode()
{
  switch(mode)
  {
    case OFF:
    printf("MODE=OFF\r\n");
    break;

    case CLOCK:
    printf("MODE=CLOCK\r\n");
    break;

    case ANIMATION:
    printf("MODE=ANIMATION\r\n");
    break;
  }
}
/// Private

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
  int minuteOffset = 0;
  int hourOffset = 0;

  switch(runCnt)
  {
    case 0:
    switchLedOn(getSecondLedNr(clockTime[SECOND]));
    break;

    case 1:
    switchLedOn(getMinuteLedNr(clockTime[MINUTE]));
    break;

    case 2:
    switchLedOn(getHourLedNr(clockTime[HOUR]));
    break;
  }

  runCnt++;
  if(runCnt > 2)
  {
    runCnt = 0;
  }
}

int displayBuffer::getSecondLedNr(int second)
{
  second = poseOffset(second, 60);

  int div = second/5;
  int remainder = second%5;

  return  ((div*12+remainder+6) + (totalNrLeds/2))%totalNrLeds;
}

int displayBuffer::getMinuteLedNr(int minute)
{
  minute = poseOffset(minute, 60);

  int div = minute/5;
  int remainder = minute%5;

  return ((div*12+remainder+1) + (totalNrLeds/2))%totalNrLeds;
}

int displayBuffer::getHourLedNr(int hour)
{
  hour = poseOffset(hour, 12);
  return ((hour+6)*12)%totalNrLeds;
}

int displayBuffer::poseOffset(int org, int span)
{
  switch(pose)
  {
    case UP:
    org = (org + 0)%span;
    break;

    case DOWN:
    org = (org + span/2)%span;
    break;

    case LEFT:
    org = (org + (3*span)/4)%span;
    break;

    case RIGHT:
    org = (org + span/4)%span;
    break;
  }
  return org;
}

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