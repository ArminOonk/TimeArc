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

CTL_TASK_t mainTask, ledTask;
static unsigned ledTaskStack[256];

displayBuffer display;

capTouch touch2(GPIOC, GPIO_Pin_3);
capTouch touch3(GPIOC, GPIO_Pin_4);
capTouch touch4(GPIOD, GPIO_Pin_2);
capTouch touch5(GPIOB, GPIO_Pin_5);
unsigned int ledCnt = 0;

void ledHandler(void *p)
{
  time_t currentTime = 1401312421;
  CTL_TIME_t startupTime = ctl_get_current_time();

  touch2.init();
  touch3.init();
  touch4.init();
  touch5.init();

  int touch2Cnt = 0;
  int touch3Cnt = 0;

  while(true)
  {
    //time_t now = ((ctl_get_current_time()-startupTime)/1000) + currentTime;
    //display.setTime(now);

    touch2.start();
    touch3.start();
    touch4.start();
    touch5.start();

    ctl_delay(50);
    if(touch2.isTouched())
    {
      touch2Cnt++;
      if(touch2Cnt > 59)
      {
        touch2Cnt = touch2Cnt - 60;
      }
    }   

    if(touch3.isTouched())
    {
      touch2Cnt--;
      if(touch2Cnt < 0)
      {
        touch2Cnt = 60 + touch2Cnt;
      }
    }

    if(touch4.isTouched())
    {
      touch3Cnt++;
      if(touch3Cnt > 59)
      {
        touch3Cnt -= 60;
      }
    }

    if(touch5.isTouched())
    {
      touch3Cnt--;
      if(touch3Cnt < 0)
      {
        touch3Cnt += 60;
      }
    }

    display.minuteOn(touch2Cnt);
    display.secondOn(touch3Cnt);
    display.switchBuffer();
    
    touch2.stop();
    touch3.stop();

    ctl_delay(10); 
    ledCnt++;
  }
}

// MAIN
int main(void)
{
  SystemInit();
  RCC_PCLK1Config(RCC_HCLK_Div1); 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 
  // Configure GPIO
  initGPIO();

  // CTL
  ctl_start_timer(ctl_increment_tick_from_isr);
  ctl_time_increment = 1;
  
  ctl_task_init(&mainTask, 255, "main");
  memset(ledTaskStack, 0xba, sizeof(ledTaskStack));

  // Make another task ready to run.
  ctl_task_run(&ledTask, 1, ledHandler, 0, "ledTask", sizeof(ledTaskStack) / sizeof(unsigned), ledTaskStack, 0);

  // Now all the tasks have been created go to lowest priority.
  ctl_task_set_priority(&mainTask, 0);

  while(1)
  {
    if(ledTaskStack[0]  !=  0xbabababa)
      error("ledTaskStack Overflow!");
  }
}


void error(const char *err)
{
  while(1) //loop forever
  {
    
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
