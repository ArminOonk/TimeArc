// STARTUP_FROM_RESET defineer  in project properties bij preprocessor definitions
// Dit slaat het programma permanent in de flash
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <ctl_api.h>
#include "main.h"

//FUNCTIONS
void ledHandler(void *p);
void ledsOff();
void ctl_delay(CTL_TIME_t t);


CTL_TASK_t mainTask, ledTask;
static unsigned ledTaskStack[256];

void ledHandler(void *p)
{
  while(true)
  {
    ctl_delay(1000);
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
  if(SysTick_Config(SystemCoreClock / 1000))
   error("SysTick Error"); 

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

void ledsOff()
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

  ledsOff();
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

/*void initTimer(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    
  TIM_OCInitTypeDef TIM_OCInitStructure;

  // TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_Period            = 255;
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
  
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  TIM_Cmd(TIM2, ENABLE); 

  // TIM3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
  
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  TIM_Cmd(TIM3, ENABLE); 

  // TIM4
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
  
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM4, ENABLE);
  TIM_Cmd(TIM4, ENABLE);

  // Initial values
  ARCDOZEN1_PWM(0);
  ARCDOZEN2_PWM(0);
  ARCDOZEN3_PWM(0);
  ARCDOZEN4_PWM(0);
  ARCDOZEN5_PWM(0);
  ARCDOZEN6_PWM(0);
  ARCDOZEN7_PWM(0);
  ARCDOZEN8_PWM(0);
  ARCDOZEN9_PWM(0);
  ARCDOZEN10_PWM(0);
  ARCDOZEN11_PWM(0);
  ARCDOZEN12_PWM(0);
}*/

void ctl_handle_error(CTL_ERROR_CODE_t error)
{
  //Do something
}

void ctl_delay(CTL_TIME_t t)
{
  ctl_timeout_wait(ctl_get_current_time()+t);
}
