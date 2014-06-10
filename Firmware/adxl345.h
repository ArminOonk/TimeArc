#ifndef ADXL345
#define ADXL345

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <misc.h>
#include <math.h>
#include <stdio.h>
#include "defines.h"

#define M_PI           3.14159265358979323846

#define SPI_CS_PORT GPIOA
#define SPI_CS_PIN  GPIO_Pin_4
#define SPI_CS_HIGH SPI_CS_PORT->BRR |= SPI_CS_PIN
#define SPI_CS_LOW SPI_CS_PORT->BSRR |= SPI_CS_PIN

#define ACCEL_INT2_PORT   GPIOC
#define ACCEL_INT2_PIN   GPIO_Pin_1

#define POWER_CTL 0x2D    //Power Control Register
#define DATA_FORMAT 0x31
#define DATAX0 0x32   //X-Axis Data 0
#define INT_MAP 0x2f
#define TAP_AXES 0x2A
#define THRESH_TAP 0x1d
#define DURATION 0x21
#define LATENT 0x22
#define WINDOW 0x23
#define INT_ENABLE 0x2e
#define INT_SOURCE 0x30

#define SINGLE_TAP (1<<6)
#define DOUBLE_TAP (1<<5)

class adxl345
{
  public:
  adxl345();
  void init();
  void interrupt();
  
  void readAccel();
  void print();

  short xRaw, yRaw, zRaw;
  float x, y, z;

  unsigned int intCnt;
  pose_t pose;

  private:
  char spiSendByte(char byte);
  void writeRegister(char registerAddress, char value);
  char readRegister(char registerAddress);

  void enableInterrupt();
  void disableInterrupt();

  bool updatePose();
  bool inRange(float testVal, float mid, float span);

  static const float scaleFactor = 1.0/256.0;
  static const float angleOffset = 45.0*(M_PI/180.0);

  float cosAngle;
  float sinAngle;

  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  bool interruptEnabled;
};

#endif