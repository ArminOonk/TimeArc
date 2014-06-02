#ifndef DEFINE
#define DEFINE

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
#endif
