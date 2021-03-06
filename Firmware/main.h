#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <misc.h>
#include <ctl_api.h>
#include <stdio.h>
#include <time.h>

#include "defines.h"
#include "displayBuffer.h"
#include "capTouch.h"
#include "adxl345.h"
#include "RtcClock.h"
#include "lightSensor.h"
#include "WString.h"

void initGPIO(void);
void initTimer(void);
void ctl_handle_error(CTL_ERROR_CODE_t error);
void error(const char *err);
