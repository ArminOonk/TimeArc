#include "adxl345.h"
extern rtcClock rtc;

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

  GPIO_InitStructure.GPIO_Pin   = ACCEL_INT1_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ACCEL_INT1_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = ACCEL_INT2_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ACCEL_INT2_PORT, &GPIO_InitStructure);
  
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
        
  writeRegister(THRESH_TAP, 0x40);        
  writeRegister(DURATION, 0x30);           
  writeRegister(LATENT, 0x40 );          
  writeRegister(WINDOW, 0xff);
  writeRegister(TAP_AXES, 0x07);

  writeRegister(POWER_CTL, 0x08);   //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeRegister(INT_MAP, 0x00);  // Send DOUBLE_TAP to INT2
  writeRegister(INT_ENABLE, 0b01100000);
  
  readRegister(INT_SOURCE); //Clear the interrupts from the INT_SOURCE register.

  readAccel();
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

  if(ACCCEL_INT1_DETECTED)
  {
    char intSource =  readRegister(INT_SOURCE);
    bool print = false;

    if(intSource & (1<<5))
    { 
      printf("ACCEL=DOUBLETAP\r\n");
      print = true;
      lastUserActionTime = rtc.getUTC();
    }else if(intSource & (1<<6))
    { 
      printf("ACCEL=TAP\r\n");
      print = true;
      lastUserActionTime = rtc.getUTC();
    }

    if(!print)
    {
      printf("ACCEL=INT1_%2x\r\n", intSource);
    }
  }

  if(ACCCEL_INT2_DETECTED)
  {
    char intSource =  readRegister(INT_SOURCE);
    bool print = false;
    if(!print)
    {
       printf("ACCEL=INT2_%2x\r\n", intSource);
    }
  }
  /*if(!interruptEnabled)
  {
    enableInterrupt();
  }*/
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

bool adxl345::inRange(float testVal, float mid, float span)
{
  if(testVal > (mid - span) && testVal < (mid + span))
  {
    return true;
  }
  return false;
}

void adxl345::print()
{
  switch(pose)
  {
    case UP:
    printf("POSE=UP\r\n");
    break;

    case DOWN:
    printf("POSE=DOWN\r\n");
    break;

    case LEFT:
    printf("POSE=LEFT\r\n");
    break;

    case RIGHT:
    printf("POSE=RIGHT\r\n");
    break;

    case FRONT:
    printf("POSE=FRONT\r\n");
    break;

    case BACK:
    printf("POSE=BACK\r\n");
    break;
  }
}

time_t adxl345::lastUserAction()
{
  return lastUserActionTime;
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
    print();
    return true;
  }
  return false;
}