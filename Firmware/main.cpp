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

unsigned int ledCnt = 0;

unsigned int touch2Time = 0;
void ledHandler(void *p)
{
  time_t currentTime = 1401312421;
  CTL_TIME_t startupTime = ctl_get_current_time();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef touch2OutputStruct; 
  GPIO_StructInit(&touch2OutputStruct);
  touch2OutputStruct.GPIO_Pin   = TOUCH2_PIN;
  touch2OutputStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
  touch2OutputStruct.GPIO_Speed = GPIO_Speed_50MHz;
  
  // Configure the touch sensor as input
  GPIO_InitTypeDef touch2InputStruct; 
  GPIO_StructInit(&touch2InputStruct);
  touch2InputStruct.GPIO_Pin   = TOUCH2_PIN;
  touch2InputStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  touch2InputStruct.GPIO_Speed = GPIO_Speed_50MHz;

  // Set touchpin Low
  GPIO_Init(TOUCH2_PORT, &touch2OutputStruct);
  TOUCH2_LOW;

  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure; 
  EXTI_StructInit(&EXTI_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
  EXTI_InitStructure.EXTI_Line     = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode     = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger  = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannel                    = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  while(true)
  {
    time_t now = ((ctl_get_current_time()-startupTime)/1000) + currentTime;
    display.setTime(now);
   
    TIM3->CNT = 0;                                // Set timer to zero
    touch2Time = 0;

    GPIO_Init(TOUCH2_PORT, &touch2InputStruct);   // Configure touch1 as input pull up

    ctl_delay(50);
    if(touch2Time != 0 && touch2Time < 25000)
    {
      //Touch!
      startupTime = ctl_get_current_time();
    }   

    // Set touchpin Low
    GPIO_Init(TOUCH2_PORT, &touch2OutputStruct);
    TOUCH2_LOW;

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
