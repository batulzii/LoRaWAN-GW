#ifndef _OPT3001_H_
#define _OPT3001_H_

#include "typedefs.h"
#include "i2c.h"

#define OPT3001_ADDR	  0x44 //1000100. Do module chan Address duoc noi xuong GND
#define REG_RESULT			0x00
#define REG_CONFIG			0x01
#define REG_LOW_LM			0x02
#define REG_HIGH_LM			0x03
#define REG_MAN_ID			0x7E
#define REG_DEVIC_ID		0x7F

u16 opt3001_LUX(void);

#endif
