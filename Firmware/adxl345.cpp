#include "adxl345.h"

adxl345::adxl345()
{
  xRaw = 0;
  yRaw = 0;
  zRaw = 0;

  x = 0.0;
  y = 0.0;
  z = 0.0;

  cosAngle = cos(angleOffset);
  sinAngle = sin(angleOffset);

  pose = UNKNOWN;

  intCnt = 0;
  interruptEnabled = false;

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
  writeRegister(DATA_FORMAT, 0b00001010);     // Range +/- 2g
        
  writeRegister(THRESH_TAP, 0x30);        
  writeRegister(DURATION, 0x7f);           
  writeRegister(LATENT, 0x10 );          
  writeRegister(WINDOW, 0x10);
  writeRegister(TAP_AXES, 0x0f);

  writeRegister(POWER_CTL, 0x08);   //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.

  readAccel();
}

int source = 0;
void adxl345::interrupt()
{
  EXTI_ClearITPendingBit(EXTI_Line1);

  if(readRegister(INT_SOURCE) & (1<<5))
  {
    // double tap
    source = 2;
  }
  else
  {
    // single tap
    source = 1;
  }

  //disableInterrupt();

  intCnt++;
}

void adxl345::readAccel()
{
  char xLow = readRegister(DATAX0);
  char xHigh = readRegister(DATAX0+1);
  char yLow = readRegister(DATAX0+2);
  char yHigh = readRegister(DATAX0+3);
  char zLow = readRegister(DATAX0+4);
  char zHigh = readRegister(DATAX0+5);

  xRaw = (xHigh<<8)| xLow;
  yRaw = (yHigh<<8)|yLow;
  zRaw = (zHigh<<8)|zLow;

  x = scaleFactor*(cosAngle*xRaw - sinAngle*yRaw);
  y = -1.0*scaleFactor*(sinAngle*xRaw + sinAngle*yRaw);
  z = -zRaw*scaleFactor;

  updatePose();
  /*if(!interruptEnabled)
  {
    enableInterrupt();
  }*/
}

// Private

void adxl345::enableInterrupt()
{
  interruptEnabled = true;

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

  writeRegister(INT_MAP, SINGLE_TAP|DOUBLE_TAP);   //Send the Tap and Double Tap Interrupts to INT2 pin
  writeRegister(INT_ENABLE, 0x60 );
  //readRegister(INT_SOURCE); //Clear the interrupts from the INT_SOURCE register.
}

void adxl345::disableInterrupt()
{
  interruptEnabled = false;

  EXTI_InitStructure.EXTI_Line     = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode     = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger  = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd  = DISABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannel                    = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = DISABLE;

  NVIC_Init(&NVIC_InitStructure);

  writeRegister(INT_MAP, 0x00);   //Send the Tap and Double Tap Interrupts to INT2 pin
  writeRegister(INT_ENABLE, 0x00);
  
}

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

bool adxl345::inRange(float testVal, float mid, float span)
{

  if(testVal > (mid - span) && testVal < (mid + span))
  {
    return true;
  }
  return false;
}

bool adxl345::updatePose()
{
pose_t tempPose = UNKNOWN;

  if(inRange(x, 1.0, 0.2) && inRange(y, 0.0, 0.4)  && inRange(z, 0.0, 0.4) )
  { 
    tempPose = RIGHT;
  }

  if(inRange(x, -1.0, 0.2) && inRange(y, 0.0, 0.4)  && inRange(z, 0.0, 0.4) )
  { 
    tempPose = LEFT;
  }

  if(inRange(x, 0.0, 0.4) && inRange(y, 1.0, 0.2)  && inRange(z, 0.0, 0.4) )
  { 
    tempPose = UP;
  }

  if(inRange(x, 0.0, 0.4) && inRange(y, -1.0, 0.2)  && inRange(z, 0.0, 0.4) )
  { 
    tempPose = DOWN;
  }

  if(inRange(x, 0.0, 0.4) && inRange(y, 0.0, 0.4)  && inRange(z, 1.0, 0.2) )
  { 
    tempPose = BACK;
  }

  if(inRange(x, 0.0, 0.4) && inRange(y, 0.0, 0.4)  && inRange(z, -1.0, 0.2) )
  { 
    tempPose = FRONT;
  }

  if(tempPose != pose && tempPose != UNKNOWN)
  {
    pose = tempPose;
    switch(pose)
    {
      case UP:
      printf("pose=UP\r\n");
      break;

      case DOWN:
      printf("pose=DOWN\r\n");
      break;

      case LEFT:
      printf("pose=LEFT\r\n");
      break;

      case RIGHT:
      printf("pose=RIGHT\r\n");
      break;

      case FRONT:
      printf("pose=FRONT\r\n");
      break;

      case BACK:
      printf("pose=BACK\r\n");
      break;

    }
    return true;
  }
  return false;
}