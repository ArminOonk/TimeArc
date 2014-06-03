#include "adxl345.h"

adxl345::adxl345()
{
  x = 0;
  y = 0;
  z = 0;

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

void adxl345::init()
{
  writeRegister(DATA_FORMAT, 0x00);    // Range +/- 2g
  
  writeRegister(INT_MAP, (1<<5)|(1<<6));    //Send the Tap and Double Tap Interrupts to INT2 pin
  writeRegister(TAP_AXES, 0x01);   //Look for taps on the Z axis only.
  writeRegister(THRESH_TAP, 84); //Set the Tap Threshold to 56=3g, 84=4.5g
  writeRegister(DURATION, 0x10);   //Set the Tap Duration that must be reached
  writeRegister(LATENT, 0x50);     //100ms Latency before the second tap can occur.
  writeRegister(WINDOW, 0xFF);

  //Enable the Single and Double Taps.
  writeRegister(INT_ENABLE, 0xE0);
  writeRegister(POWER_CTL, 0x08);   //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  readRegister(INT_SOURCE); //Clear the interrupts from the INT_SOURCE register.

  readAccel();
}

void adxl345::interrupt()
{
  EXTI_ClearITPendingBit(EXTI_Line1);
  if(readRegister(INT_SOURCE) & (1<<5))
  {
    // double tap
  }
  else
  {
    // single tap
  }
}

void adxl345::readAccel()
{
    x = (readRegister(DATAX0+1)<<8)|readRegister(DATAX0);
    y = (readRegister(DATAX0+3)<<8)|readRegister(DATAX0+2);
    z = (readRegister(DATAX0+5)<<8)|readRegister(DATAX0+4);
}

// Private

char adxl345::spiSendByte(char byte)
{
  // Loop while DR register in not empty 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  // Send byte through the SPI1 peripheral 
  SPI_I2S_SendData(SPI1, byte);

  // Wait to receive a byte 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  return SPI_I2S_ReceiveData(SPI1);
}

void adxl345::writeRegister(char registerAddress, char value)
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

char adxl345::readRegister(char registerAddress)
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
