// STARTUP_FROM_RESET defineer  in project properties bij preprocessor definitions
// Dit slaat het programma permanent in de flash
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <ctl_api.h>
#include "main.h"
#include "displayBuffer.h"

//FUNCTIONS
void ledHandler(void *p);
void ctl_delay(CTL_TIME_t t);

CTL_TASK_t mainTask, ledTask;
static unsigned ledTaskStack[256];

displayBuffer display;
unsigned int ledCnt = 0;
void ledHandler(void *p)
{

  while(true)
  {
    display.hourOn(ledCnt%12);
    display.minuteOn(ledCnt%60);
    display.secondOn(ledCnt%60);
    display.switchBuffer();

    ctl_delay(1000);
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
  initTimer();

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
/*
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
  GPIO_Init(ARCDOZEN11_PORT, &GPIO_InitStructure);*/
}

void initTimer(void)
{
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
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  NVIC_InitStructure.NVIC_IRQChannel                    = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM2, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void ctl_handle_error(CTL_ERROR_CODE_t error)
{
  //Do something
}

void ctl_delay(CTL_TIME_t t)
{
  ctl_timeout_wait(ctl_get_current_time()+t);
}
