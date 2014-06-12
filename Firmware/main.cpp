// STARTUP_FROM_RESET defineer  in project properties bij preprocessor definitions
// Dit slaat het programma permanent in de flash
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <ctl_api.h>
#include "main.h"

//FUNCTIONS
void ledHandler(void *p);
void ctl_delay(CTL_TIME_t t);
void initADC();
void initRTC();
void initUsart();
unsigned int readLight();
extern "C" int __putchar(char ch);
void handleTouch();
void printAutoMode();

CTL_TASK_t mainTask, ledTask, touchTask, comTask;
static unsigned ledTaskStack[256];
static unsigned touchTaskStack[256];
static unsigned comTaskStack[256];

displayBuffer display;

capTouch touchDown(GPIOC, GPIO_Pin_13);
capTouch touchRight(GPIOC, GPIO_Pin_3);
capTouch touchLeft(GPIOC, GPIO_Pin_4);
capTouch touchUp(GPIOD, GPIO_Pin_2);
capTouch touchTop(GPIOB, GPIO_Pin_5);
unsigned int ledCnt = 0;

//float light = 0;
adxl345 accel;
rtcClock rtc;
lightSensor light;

String completeData;
bool autoMode = true;
bool autoLight = true;
float manualLightVal = 0.0f;

void comHandler(void *p)
{

  while(true)
  {
    if(completeData.length() > 0)
    {
        //Process data
        if(completeData.startsWith("TIME?"))
        {
          rtc.printTime();
        }
        else if(completeData.startsWith("TIME="))
        {
          String time = completeData.substring(completeData.indexOf("=")+1);
          time_t t = time.toInt();
          rtc.setTime(t);
        }
        else if(completeData.startsWith("OFFSET?"))
        {
          rtc.printOffset();
        }
        else if(completeData.startsWith("OFFSET="))
        {
          String tzString = completeData.substring(completeData.indexOf("=")+1);
          int offset = tzString.toInt();
          rtc.setOffset(offset);
        }
        else if(completeData.startsWith("POSE?"))
        {
          accel.print();
        }
        else if(completeData.startsWith("ID?"))
        {
          printf("ID=");
          for(int i=0; i<12; i++)
          {
            printf("%02x", *(const char *)(0x1FFFF7E8+i));
          }
          printf("\r\n");
        }
        else if(completeData.startsWith("MODE?"))
        {
          display.printMode();
        }
        else if(completeData.startsWith("MODE="))
        {
          String modeString = completeData.substring(completeData.indexOf("=")+1);
          if(modeString == "OFF")
          {
            display.setMode(OFF);
          }
          else if(modeString == "CLOCK")
          {
            display.setMode(CLOCK);
          }
          else if(modeString == "ANIMATION")
          {
            display.setMode(ANIMATION);
          }
        }
        else if(completeData.startsWith("AUTO?"))
        {
          printAutoMode();
        }
        else if(completeData.startsWith("AUTO="))
        {
          String autoString = completeData.substring(completeData.indexOf("=")+1);
          autoMode = (autoString == "TRUE");
          printAutoMode();
        }
        else if(completeData.startsWith("LIGHT?"))
        {
          printf("LIGHT=%0.2f\r\n", light.getValue());
        }
        else if(completeData.startsWith("LIGHT="))
        {
          String lightString = completeData.substring(completeData.indexOf("=")+1);
          if(lightString == "AUTO")
          {
            autoLight = true;
          }
          else
          {
            manualLightVal = lightString.toFloat();
            autoLight = false;
          }

        }
        completeData = "";
    }
    ctl_delay(1);
  }
}

void ledHandler(void *p)
{
  time_t currentTime = 1401312421;
  CTL_TIME_t startupTime = ctl_get_current_time();

  ctl_delay(1000);
  accel.init();
 
  while(true)
  {
    handleTouch();

    accel.readAccel();
    display.setPose(accel.pose);

    float lightVal = light.getValue();

    if(autoMode)
    {
      switch(accel.pose)
      {
        case FRONT:
        display.setMode(OFF);
        break;

        case BACK:
        display.setMode(ANIMATION);
        break;

        default:
        display.setMode(CLOCK);
        break;
      }
    }

    if(display.getMode() == CLOCK)
    {
      display.setTime(rtc.getTime());
      if(autoLight)
      {
        display.setIntensity(lightVal);
      }
      else
      {
        display.setIntensity(manualLightVal);
      }
    }
    else if(display.getMode() == ANIMATION)
    {
      for(int i=0; i<totalNrLeds; i+=4)
      {
        display.ledOn(i+ledCnt%4);
      }
      display.setIntensity(1.0);
      display.switchBuffer();
    }

    ledCnt++;
    ctl_delay(100); 
  }
}

const int touchDelay = 50;
void touchHandler(void *p)
{
  touchDown.init();
  touchRight.init();
  touchLeft.init();
  touchUp.init();
  touchTop.init();

  while(true)
  {
    touchDown.start();
    ctl_delay(touchDelay);
    touchDown.stop();

    touchRight.start();
    ctl_delay(touchDelay);
    touchRight.stop();

    touchLeft.start();
    ctl_delay(touchDelay);
    touchLeft.stop();

    touchUp.start();
    ctl_delay(touchDelay);
    touchUp.stop();

    touchTop.start();
    ctl_delay(touchDelay);
    touchTop.stop();
  }
}

// MAIN
int main(void)
{
  SystemInit();
  RCC_PCLK1Config(RCC_HCLK_Div1); 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 
  initGPIO();   // Configure GPIO
  initUsart();

  rtc.init();

  printf("\r\n");
  printf("start=%s %s\r\n", __DATE__, __TIME__);
  // CTL
  ctl_start_timer(ctl_increment_tick_from_isr);
  ctl_time_increment = 1;
  
  ctl_task_init(&mainTask, 255, "main");
  memset(ledTaskStack, 0xba, sizeof(ledTaskStack));
  memset(touchTaskStack, 0xba, sizeof(touchTaskStack));
  memset(comTaskStack, 0xba, sizeof(comTaskStack));

  // Make another task ready to run.
  ctl_task_run(&ledTask, 2, ledHandler, 0, "ledTask", sizeof(ledTaskStack) / sizeof(unsigned), ledTaskStack, 0);
  ctl_task_run(&touchTask, 3, touchHandler, 0, "touchTask", sizeof(touchTaskStack) / sizeof(unsigned), touchTaskStack, 0);
  ctl_task_run(&comTask, 1, comHandler, 0, "comTask", sizeof(comTaskStack) / sizeof(unsigned), comTaskStack, 0);
  // Now all the tasks have been created go to lowest priority.
  ctl_task_set_priority(&mainTask, 0);

  while(1)
  {
    if(ledTaskStack[0]  !=  0xbabababa)
      error("ledTaskStack Overflow!");
    if(touchTaskStack[0]  !=  0xbabababa)
      error("touchTaskStack Overflow!");
    if(comTaskStack[0]  !=  0xbabababa)
      error("comTaskStack Overflow!");
  }
}


void error(const char *err)
{
  while(true) //loop forever
  {
    printf("ERROR: %s\r\n", err);
    NVIC_SystemReset();
  }
}

void initGPIO(void)
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

}

void ctl_handle_error(CTL_ERROR_CODE_t error)
{
  //Do something
}

void ctl_delay(CTL_TIME_t t)
{
  ctl_timeout_wait(ctl_get_current_time()+t);
}

void initUsart()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0x0f;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0x0f;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  // Configure USARTy
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

int __putchar(char ch)
{
  // Place your implementation of fputc here
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  ;

  return ch;
}

unsigned int lastMenuTime = 0;
void handleTouch()
{
  // Reporting
  if(touchTop.isTouched())
  {
    printf("BUTTON=TOP\r\n");
  }
  
  if(touchRight.isTouched())
  {
    printf("BUTTON=RIGHT\r\n");
  }

  if(touchLeft.isTouched())
  {
    printf("BUTTON=LEFT\r\n");
  }

  if(touchUp.isTouched())
  {
    printf("BUTTON=UP\r\n");
  }

  if(touchDown.isTouched())
  {
    printf("BUTTON=DOWN\r\n");
  }

  // change time
  unsigned int now = rtc.getUTC();
  if(touchTop.isTouched())
  {
    lastMenuTime = now;
  }

  if( (now - lastMenuTime) < 3)
  {
    if(touchRight.isTouched())
    {
      unsigned int newNow = now+3600;
      rtc.setTime(newNow);
      lastMenuTime = newNow;
    }   

    if(touchLeft.isTouched())
    {
      unsigned int newNow = now-3600;
      rtc.setTime(newNow);
      lastMenuTime = newNow;
    }

    if(touchUp.isTouched())
    {
      unsigned int newNow = now+60;
      rtc.setTime(newNow);
      lastMenuTime = newNow;
    }

    if(touchDown.isTouched())
    {
      unsigned int newNow = now-60;
      rtc.setTime(newNow);
      lastMenuTime = newNow;
    }
  }
}

void printAutoMode()
{
  if(autoMode)
  {
    printf("AUTO=TRUE\r\n");
  }
  else
  {
    printf("AUTO=FALSE\r\n");
  }
}