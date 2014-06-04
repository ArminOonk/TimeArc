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
void initRTC();
unsigned int readLight();

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

    accel.readAccel();
    display.setPose(accel.pose);

    switch(accel.pose)
    {
      case FRONT:
      display.setMode(OFF);
      break;

      case BACK:
      display.setMode(ANIMATION);
      for(int i=0; i<totalNrLeds; i+=4)
      {
        display.ledOn(i+ledCnt%4);
      }
      display.setIntensity(1.0);
      display.switchBuffer();
      break;

      default:
      display.minuteOn(touch2Cnt);
      display.secondOn(touch3Cnt);
      display.hourOn(hourCnt%12);

      light += 0.2*(readLight() - light);
      display.setIntensity(1.0-light/4096.0);

      display.setMode(CLOCK);
      break;
    }

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
  //initRTC();

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

void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

void initRTC()
{
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    RTC_Configuration();
    // Get the time one way or another
    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }

  // Check if the Power On Reset flag is set */
  if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
  {
  }
  // Check if the Pin Reset flag is set
  else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
  {
  }

  // Wait for RTC registers synchronization
  RTC_WaitForSynchro();

  //Enable the RTC Second
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  //Wait until last write operation on RTC registers has finished
  RTC_WaitForLastTask();

  // Clear reset flags
  RCC_ClearFlag();
}