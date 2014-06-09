#include "RtcClock.h"

rtcClock::rtcClock()
{

}

bool rtcClock::init()
{
  unsigned int magicNumber2 = BKP_ReadBackupRegister(BKP_DR1);

 
  if (magicNumber2 != magicNumber)
  {
    printf("RTC not configure\r\n");
    RTC_Configuration();
    printf("Done RTC configure\r\n");

    // Get the time one way or another
    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);

    setTime(1402036753);
  }

  // Check if the Power On Reset flag is set */
  if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
  {
    printf("RTC Power On Reset\r\n");
  }
  // Check if the Pin Reset flag is set
  else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
  {
    printf("RTC Pin Reset\r\n");
  }

  // Wait for RTC registers synchronization
  RTC_WaitForSynchro();

  //Wait until last write operation on RTC registers has finished
  RTC_WaitForLastTask();

  // Clear reset flags
  RCC_ClearFlag();
}

void rtcClock::setTime(time_t t)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  PWR_BackupAccessCmd(ENABLE);

  // Wait until last write operation on RTC registers has finished
  RTC_WaitForLastTask();
  // Change the current time
  RTC_SetCounter(t);
  // Wait until last write operation on RTC registers has finished
  RTC_WaitForLastTask();
}

time_t rtcClock::getTime()
{
  RTC_GetCounter();
}

void rtcClock::RTC_Configuration(void)
{
  // Enable PWR and BKP clocks
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  // Allow access to BKP Domain
  PWR_BackupAccessCmd(ENABLE);

  // Reset Backup Domain
  BKP_DeInit();

  // Enable LSE
  RCC_LSEConfig(RCC_LSE_ON);

  // Wait till LSE is ready
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    ;

  // Select LSE as RTC Clock Source
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  // Enable RTC Clock
  RCC_RTCCLKCmd(ENABLE);

  // Wait for RTC registers synchronization
  RTC_WaitForSynchro();

  // Wait until last write operation on RTC registers has finished
  RTC_WaitForLastTask();

  // Set RTC prescaler: set RTC period to 1sec 
  RTC_SetPrescaler(32767);

  // Wait until last write operation on RTC registers has finished
  RTC_WaitForLastTask();
}