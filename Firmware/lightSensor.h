#ifndef LIGHT_SENSOR
#define LIGHT_SENSOR

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

class lightSensor
{
  public:
  lightSensor();

  unsigned int read();
  float getValue();

  private:
  float filter;
  float light;
  
};

#endif