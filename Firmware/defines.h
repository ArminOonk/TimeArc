#ifndef DEFINE
#define DEFINE

#define SPI_CS_PORT GPIOA
#define SPI_CS_PIN  GPIO_Pin_4
#define SPI_CS_HIGH SPI_CS_PORT->BRR |= SPI_CS_PIN
#define SPI_CS_LOW SPI_CS_PORT->BSRR |= SPI_CS_PIN

#define POWER_CTL 0x2D    //Power Control Register
#define DATA_FORMAT 0x31
#define DATAX0 0x32   //X-Axis Data 0
#endif
