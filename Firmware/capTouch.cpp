#include "capTouch.h"

capTouch::capTouch(GPIO_TypeDef* _port, uint32_t _pin)
{
  port = _port;
  pin = _pin;
  EXTI_StructInit(&EXTI_InitStructure);
}

void capTouch::init()
{
  configurePort();
  configurePin();

  RCC_APB2PeriphClockCmd(portClock, ENABLE);
  
  // TIMER3 is control loop timer
  // Timer3 loopt op 72Mhz
  // prescaler = 144; -> timer freq is 500kHz
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseStructure.TIM_Prescaler         = 144;
  TIM_TimeBaseStructure.TIM_Period            = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_Cmd(TIM3, ENABLE);

  GPIO_StructInit(&inputStruct);
  inputStruct.GPIO_Pin   = pin;
  inputStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  inputStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_StructInit(&outputStruct);
  outputStruct.GPIO_Pin   = pin;
  outputStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
  outputStruct.GPIO_Speed = GPIO_Speed_50MHz;

  if(port == GPIOC && pin == GPIO_Pin_13)
  {
    inputStruct.GPIO_Pin   = pin;
    inputStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    inputStruct.GPIO_Speed = GPIO_Speed_2MHz;

    outputStruct.GPIO_Pin   = pin;
    outputStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    outputStruct.GPIO_Speed = GPIO_Speed_2MHz;
  }
  stop(); // Set pin low 

  initExti();
}

void capTouch::interrupt()
{
  if(EXTI_GetITStatus(EXTI_InitStructure.EXTI_Line) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_InitStructure.EXTI_Line);
    touchTime = (TIM3->CNT - startTime)&0xffff;
  }
}

void capTouch::start()
{
  startTime = TIM3->CNT;
  touchTime = 0;
  GPIO_Init(port, &inputStruct);
}

void capTouch::stop()
{
  GPIO_Init(port, &outputStruct);
  pinLow();  
}

bool capTouch::isTouched()
{
  return (touchTime != 0 && touchTime < 35000);
}

// Private

void capTouch::configurePort()
{
  if(port == GPIOA)
  {
   portClock = RCC_APB2Periph_GPIOA;
   portSource = GPIO_PortSourceGPIOA;
  }
  else if(port == GPIOB)
  {  
    portClock = RCC_APB2Periph_GPIOB;
    portSource = GPIO_PortSourceGPIOB;
  }
  else if(port == GPIOC)
  {
    portClock = RCC_APB2Periph_GPIOC;
    portSource = GPIO_PortSourceGPIOC;
  }
  else if(port == GPIOD)
  {
    portClock = RCC_APB2Periph_GPIOD;
    portSource = GPIO_PortSourceGPIOD;
  }
  else if(port == GPIOE)
  {
    portClock = RCC_APB2Periph_GPIOE;
    portSource = GPIO_PortSourceGPIOE;
  }
  else if(port == GPIOF)
  {
    portClock = RCC_APB2Periph_GPIOF;
    portSource = GPIO_PortSourceGPIOF;
  }
  else if(port == GPIOG)
  {
    portClock = RCC_APB2Periph_GPIOG;
    portSource = GPIO_PortSourceGPIOG;
  }
}

void capTouch::configurePin()
{
  if(pin == GPIO_Pin_0)
  {
    pinSource = GPIO_PinSource0;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  
    EXTI_InitStructure.EXTI_Line     = EXTI_Line0;
  }
  else if(pin == GPIO_Pin_1)
  {
    pinSource = GPIO_PinSource1;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line1;
  }
  else if(pin == GPIO_Pin_2)
  {
    pinSource = GPIO_PinSource2;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line2;
  }
  else if(pin == GPIO_Pin_3)
  {
    pinSource = GPIO_PinSource3;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line3;
  }
  else if(pin == GPIO_Pin_4)
  {
    pinSource = GPIO_PinSource4;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line4;
  }
  else if(pin == GPIO_Pin_5)
  {
    pinSource = GPIO_PinSource5;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line5;
  }
  else if(pin == GPIO_Pin_6)
  {
    pinSource = GPIO_PinSource6;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line6;
  }
  else if(pin == GPIO_Pin_7)
  {
    pinSource = GPIO_PinSource7;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line7;
  }
  else if(pin == GPIO_Pin_8)
  {
    pinSource = GPIO_PinSource8;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line8;
  }
  else if(pin == GPIO_Pin_9)
  {
    pinSource = GPIO_PinSource9;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line9;
  }
  else if(pin == GPIO_Pin_10)
  {
    pinSource = GPIO_PinSource10;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line10;
  }
  else if(pin == GPIO_Pin_11)
  {
    pinSource = GPIO_PinSource11;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line11;
  }
  else if(pin == GPIO_Pin_12)
  {
    pinSource = GPIO_PinSource12;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line12;
  }
  else if(pin == GPIO_Pin_13)
  {
    pinSource = GPIO_PinSource13;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line13;
  }
  else if(pin == GPIO_Pin_14)
  {
    pinSource = GPIO_PinSource14;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line14;
  }
  else if(pin == GPIO_Pin_15)
  {
    pinSource = GPIO_PinSource15;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    EXTI_InitStructure.EXTI_Line     = EXTI_Line15;
  }
}

void capTouch::initExti()
{
  GPIO_EXTILineConfig(portSource, pinSource);
  // EXTI_InitStructure.EXTI_Line Configured in configurePin()
  EXTI_InitStructure.EXTI_Mode     = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger  = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  //NVIC_InitStructure.NVIC_IRQChannel Configured in configurePin()
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}

void capTouch::pinLow()
{
  port->BRR |= pin;
}

void capTouch::pinHigh()
{
  port->BSRR |= pin;
}