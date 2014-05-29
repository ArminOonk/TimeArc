#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include <ctl_api.h>
#include "defines.h"
#include "displayBuffer.h"
#include "capTouch.h"

extern displayBuffer display;

extern "C" void TIM2_IRQHandler(void)
{
  ctl_enter_isr();

  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    display.run();
  }

  ctl_exit_isr();
}
/*
extern unsigned int touch1Time;
extern "C" void EXTI15_10_IRQHandler(void)
{
  ctl_enter_isr();

  if(EXTI_GetITStatus(EXTI_Line15) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line15);
    touch1Time = TIM3->CNT;
  }

  ctl_exit_isr();
}
*/

extern capTouch touch2;
extern "C" void EXTI3_IRQHandler(void)
{
  ctl_enter_isr();

  
    touch2.interrupt();
  

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
