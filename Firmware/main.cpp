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
unsigned int readLight();
//void initSPI();
//char spiSendByte(char byte);
//char readADXL345Register(char registerAddress);
//void writeADXL345Register(char registerAddress, char value);
//void readAccel();

CTL_TASK_t mainTask, ledTask, touchTask;
static unsigned ledTaskStack[256];
static unsigned touchTaskStack[256];

displayBuffer display;

capTouch touchDown(GPIOC, GPIO_Pin_13);
capTouch touchRight(GPIOC, GPIO_Pin_3);
capTouch touchLeft(GPIOC, GPIO_Pin_4);
capTouch touchUp(GPIOD, GPIO_Pin_2);
capTouch touchTop(GPIOB, GPIO_Pin_5);
unsigned int ledCnt = 0;

float light = 0;
adxl345 accel;

void ledHandler(void *p)
{
  time_t currentTime = 1401312421;
  CTL_TIME_t startupTime = ctl_get_current_time();

  ctl_delay(1000);
  accel.init();

  int touch2Cnt = 0;
  int touch3Cnt = 0;
  int hourCnt = 0;
  
  while(true)
  {
    //time_t now = ((ctl_get_current_time()-startupTime)/1000) + currentTime;
    //display.setTime(now);

    if(touchTop.isTouched())
    {
      hourCnt++;
    }

    if(touchRight.isTouched())
    {
      touch2Cnt++;
      if(touch2Cnt > 59)
      {
        touch2Cnt -= 60;
      }
    }   

    if(touchLeft.isTouched())
    {
      touch2Cnt--;
      if(touch2Cnt < 0)
      {
        touch2Cnt += 60;
      }
    }

    if(touchUp.isTouched())
    {
      touch3Cnt++;
      if(touch3Cnt > 59)
      {
        touch3Cnt -= 60;
      }
    }

    if(touchDown.isTouched())
    {
      touch3Cnt--;
      if(touch3Cnt < 0)
      {
        touch3Cnt += 60;
      }
    }

    display.minuteOn(touch2Cnt);
    display.secondOn(touch3Cnt);
    display.hourOn(hourCnt%12);

    light += 0.2*(readLight() - light);

    display.setIntensity(1.0-light/4096.0);
   
    accel.readAccel();

    ctl_delay(100); 
    ledCnt++;
  }
}

void touchHandler(void *p)
{
  touchDown.init();
  touchRight.init();
  touchLeft.init();
  touchUp.init();
  touchTop.init();

  bool disabledJTAG = false;
  while(true)
  {
    touchDown.start();
    ctl_delay(50);
    touchDown.stop();

    touchRight.start();
    ctl_delay(50);
    touchRight.stop();

    touchLeft.start();
    ctl_delay(50);
    touchLeft.stop();

    touchUp.start();
    ctl_delay(50);
    touchUp.stop();

    touchTop.start();
    ctl_delay(50);
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
  initADC();
  //initSPI();

  // CTL
  ctl_start_timer(ctl_increment_tick_from_isr);
  ctl_time_increment = 1;
  
  ctl_task_init(&mainTask, 255, "main");
  memset(ledTaskStack, 0xba, sizeof(ledTaskStack));
  memset(touchTaskStack, 0xba, sizeof(touchTaskStack));

  // Make another task ready to run.
  ctl_task_run(&ledTask, 1, ledHandler, 0, "ledTask", sizeof(ledTaskStack) / sizeof(unsigned), ledTaskStack, 0);
  ctl_task_run(&touchTask, 2, touchHandler, 0, "touchTask", sizeof(touchTaskStack) / sizeof(unsigned), touchTaskStack, 0);

  // Now all the tasks have been created go to lowest priority.
  ctl_task_set_priority(&mainTask, 0);

  while(1)
  {
    if(ledTaskStack[0]  !=  0xbabababa)
      error("ledTaskStack Overflow!");
    if(touchTaskStack[0]  !=  0xbabababa)
      error("touchTaskStack Overflow!");
  }
}


void error(const char *err)
{
  while(true) //loop forever
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

void initADC()
{
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  ADC_DeInit(ADC1);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  ADC_InitTypeDef  ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;

  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/*
void initSPI()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin   = SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);

  SPI_CS_HIGH;

  GPIO_InitStructure.GPIO_Pin   = ACCEL_INT2_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ACCEL_INT2_PORT, &GPIO_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  EXTI_InitStructure.EXTI_Line     = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode     = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger  = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannel                    = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  SPI_InitTypeDef   SPI_InitStructure;
  SPI_StructInit(&SPI_InitStructure);
  
  SPI_InitStructure.SPI_Direction           = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode                = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize            = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL                = SPI_CPOL_High;         //Idle state low
  SPI_InitStructure.SPI_CPHA                = SPI_CPHA_2Edge;       // latch on first edge
  SPI_InitStructure.SPI_NSS                 = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit            = SPI_FirstBit_MSB;
  
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);
}

char spiSendByte(char byte)
{
  // Loop while DR register in not empty 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  // Send byte through the SPI1 peripheral 
  SPI_I2S_SendData(SPI1, byte);

  // Wait to receive a byte 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  return SPI_I2S_ReceiveData(SPI1);
}

void writeADXL345Register(char registerAddress, char value)
{
  //Set Chip Select pin low to signal the beginning of an SPI packet.
  SPI_CS_LOW;
  //Transfer the register address over SPI.
  spiSendByte(registerAddress);
  //Transfer the desired register value over SPI.
  spiSendByte(value);
  //Set the Chip Select pin high to signal the end of an SPI packet.
  SPI_CS_HIGH;
}

char readADXL345Register(char registerAddress)
{
  //Set Chip Select pin low to signal the beginning of an SPI packet.
  SPI_CS_LOW;
  //Transfer the register address over SPI.
  spiSendByte(registerAddress|(1<<7));
  //Transfer the desired register value over SPI.
  char retVal = spiSendByte(0x00);
  //Set the Chip Select pin high to signal the end of an SPI packet.
  SPI_CS_HIGH;
  return retVal;
}

void readAccel()
{
    accelData.x = (readADXL345Register(DATAX0+1)<<8)|readADXL345Register(DATAX0);
    accelData.y = (readADXL345Register(DATAX0+3)<<8)|readADXL345Register(DATAX0+2);
    accelData.z = (readADXL345Register(DATAX0+5)<<8)|readADXL345Register(DATAX0+4);
}*/

unsigned int readLight()
{
  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  // Get the conversion value
  return ADC_GetConversionValue(ADC1);
}

void ctl_handle_error(CTL_ERROR_CODE_t error)
{
  //Do something
}

void ctl_delay(CTL_TIME_t t)
{
  ctl_timeout_wait(ctl_get_current_time()+t);
}
