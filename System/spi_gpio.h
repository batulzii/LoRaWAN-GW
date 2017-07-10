#ifndef _SPI_GPIO_H_
#define _SPI_GPIO_H_

//Define port and pins
#define NSS			MAKE_PIN(GPIOE_IDX, 16u)
#define SCK			MAKE_PIN(GPIOE_IDX, 17u)
#define MOSI   	MAKE_PIN(GPIOE_IDX, 18u)
#define MISO   	MAKE_PIN(GPIOE_IDX, 19u)

//Define state for pins
#define NSS0	GPIO_PinLow(NSS)
#define NSS1	GPIO_PinHigh(NSS)
#define SCK0	GPIO_PinLow(SCK)
#define SCK1	GPIO_PinHigh(SCK)
#define MOSI0	GPIO_PinLow(MOSI)
#define MOSI1	GPIO_PinHigh(MOSI)
#define MISO0	GPIO_PinLow(MISO)
#define MISO1	GPIO_PinHigh(MISO)

void SPI_Init(void);
u8 SPI_RW(u8 Buff);
u8 SPI_Read(u8 reg);
u8 SPI_Write(u8 reg, u8 value);
void SPI_Read_Buf(u8 reg, u8_p pBuf, u8 uchars);
void SPI_Write_Buf(u8 reg, u8_p pBuf, char_t uchars);


#endif
