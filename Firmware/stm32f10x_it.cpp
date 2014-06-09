#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include <ctl_api.h>
#include "defines.h"
#include "displayBuffer.h"
#include "capTouch.h"
#include "adxl345.h"
#include "WString.h"

const int maxData = 128;
String data;
extern String completeData;
extern "C" void USART1_IRQHandler(void)
{
  ctl_enter_isr();
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != 0)
  {
    char c = USART_ReceiveData(USART1);
    if(c == '\r' || c == '\n')
    {
      if(data.length() > 0)
      {
        data.toUpperCase();
        completeData = data;
        data = "";
      }
    }
    else
    {
      if(data.length() < maxData)
      {
        data += c;
      }
      else
      {
        // Clear to much data
        data = "";
      }
    }
  }
  ctl_exit_isr();
}

extern displayBuffer display;

extern "C" void TIM2_IRQHandler(void)
{
  ctl_enter_isr();

  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    display.run();
  }

  if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    display.displayOff();

  }
  ctl_exit_isr();
}

extern adxl345 accel;
extern "C" void EXTI1_IRQHandler(void)
{
  ctl_enter_isr();
  accel.interrupt();
  ctl_exit_isr();
}

extern capTouch touchUp;
extern "C" void EXTI2_IRQHandler(void)
{
  ctl_enter_isr();

  touchUp.interrupt(); 

  ctl_exit_isr();
}

extern capTouch touchRight;
extern "C" void EXTI3_IRQHandler(void)
{
  ctl_enter_isr();

  touchRight.interrupt(); 

  ctl_exit_isr();
}

extern capTouch touchLeft;
extern "C" void EXTI4_IRQHandler(void)
{
  ctl_enter_isr();

  touchLeft.interrupt(); 

  ctl_exit_isr();
}

extern capTouch touchTop;
extern "C" void EXTI9_5_IRQHandler(void)
{
  ctl_enter_isr();

  touchTop.interrupt(); 

  ctl_exit_isr();
}

extern capTouch touchDown;
extern "C" void EXTI15_10_IRQHandler(void)
{
  ctl_enter_isr();

  touchDown.interrupt(); 

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
