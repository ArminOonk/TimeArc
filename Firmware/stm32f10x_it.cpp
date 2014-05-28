#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include <ctl_api.h>
#include "defines.h"

extern void ledsOff();
unsigned int ledCnt = 0;
extern "C" void TIM2_IRQHandler(void)
{
  ctl_enter_isr();

  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    ledsOff();
    
    switch((ledCnt/12)%12)
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

    switch(ledCnt%12)
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

    ledCnt++;
  }

    ctl_exit_isr();
}


extern "C" void NMI_Handler(void){}

extern "C" void HardFault_Handler(void)
{
  NVIC_SystemReset();
  // Go to infinite loop when Hard Fault exception occurs
  while (1)
  {}
}

extern "C" void MemManage_Handler(void)
{
  // Go to infinite loop when Memory Manage exception occurs
  while (1)
  {}
}

extern "C" void BusFault_Handler(void)
{
  // Go to infinite loop when Bus Fault exception occurs 
  while (1)
  {}
}

extern "C" void UsageFault_Handler(void)
{
//   Go to infinite loop when Usage Fault exception occurs
  while (1)
  {}
}

extern "C" void DebugMon_Handler(void)
{}

extern "C" void SVC_Handler(void)
{}
