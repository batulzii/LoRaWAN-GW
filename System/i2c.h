#ifndef _I2C_H_
#define _I2C_H_

#include "typedefs.h"

#define I2C_ARR                 { DRV_I2C0_SCL, DRV_I2C0_SDA }, \
                                { DRV_I2C1_SCL, DRV_I2C1_SDA }

//#define SLAVE_I2C_ADDRESS      0x40

#define i2c_DisableAck()       g_i2cbase[i2cNumber]->C1 |= I2C_C1_TXAK_MASK

#define i2c_RepeatedStart()    g_i2cbase[i2cNumber]->C1 |= I2C_C1_RSTA_MASK;

#define i2c_Start()            g_i2cbase[i2cNumber]->C1 |= I2C_C1_TX_MASK;\
                               g_i2cbase[i2cNumber]->C1 |= I2C_C1_MST_MASK

#define i2c_Stop()             g_i2cbase[i2cNumber]->C1 &= ~I2C_C1_MST_MASK;\
                               g_i2cbase[i2cNumber]->C1 &= ~I2C_C1_TX_MASK

#define i2c_EnterRxMode()      g_i2cbase[i2cNumber]->C1 &= ~I2C_C1_TX_MASK;\
															 g_i2cbase[i2cNumber]->C1 |= I2C_C1_TXAK_MASK			//Ban dau la: I2C1->C1 &= ~I2C_C1_TXAK_MASK

#define i2c_Wait()               while((g_i2cbase[i2cNumber]->S & I2C_S_IICIF_MASK)==0) {} \
                                  g_i2cbase[i2cNumber]->S |= I2C_S_IICIF_MASK;   //Xoa co ngat

#define i2c_write_byte(data)   g_i2cbase[i2cNumber]->D = data

//#define WRITE                   0x00  /* Master write  */
//#define READ                    0x01  /* Master read */

void i2c_init(u8 byI2CNumber);
void IIC_StartTransmission (u8 SLAVE_ADDR, u8 Mode);
void I2CWriteRegister(u8 SLAVE_ADDR, u8 u8RegisterAddress, u8 u8Data);
void I2CWriteMultiRegisters(u8 SLAVE_ADDR, u8 u8RegisterAddress, u8_p data, u8 bytes);
unsigned char I2CReadRegister(u8 SLAVE_ADDR, u8 u8RegisterAddress);
void I2CReadMultiRegisters(u8 SLAVE_ADDR, u8 u8RegisterAddress, u8_p data, u8 bytes);		
#endif
	
