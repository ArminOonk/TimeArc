#ifndef CAP_TOUCH
#define CAP_TOUCH

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <misc.h>
#include "defines.h"

class capTouch
{
  public:
  capTouch(GPIO_TypeDef* _port, uint32_t _pin);
  void interrupt();
  void start();
  void stop();

  bool isTouched();

  private:
  void pinLow();
  void pinHigh();

  void configurePort();
  void configurePin();

  void initExti();

  uint32_t portClock;

  GPIO_TypeDef* port;
  uint32_t pin;

  uint8_t portSource;   // EXTI port source
  uint8_t pinSource;    // EXTI pin source

  GPIO_InitTypeDef outputStruct;
  GPIO_InitTypeDef inputStruct;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  unsigned int startTime;
  unsigned int touchTime;
};

#endif