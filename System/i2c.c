/******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author:	Huy
 *
 * Last Changed By:  $Author:  Huy$
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     
								By Huy 		$Date: 2017-2-6 	11:45:00
								By Hoang 	$Date: 2017-05-16 11:45:00
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "board.h"
#include "i2c.h"

typedef struct _i2c_ {
    u32 scl;
    u32 sda;
} i2c_t, *i2c_p;

static i2c_t const g_i2cArray[I2C_COUNT] = { I2C_ARR };
static u8 i2cNumber = 0;

/**/
/*!
 * I2C Initialization
 * Set Baud Rate and turn on I2C0
 * byI2CNumber la modul I2C duoc su dung (0 hoac 1)
 */
void i2c_init(u8 byI2CNumber)  
{
	u32_t port;
	u32_t pin;
	
	if (byI2CNumber >= I2C_COUNT) return ;
	
	i2cNumber = byI2CNumber;
	
	/* I2C Enable module clock */
	SIM->SCGC4 |= g_i2cclock[byI2CNumber];
	
	//Enables I2C module operation
	g_i2cbase[byI2CNumber]->C1 |= I2C_C1_IICEN_MASK;
	
	//Tinh gia tri thanh ghi I2Cx_F
	g_i2cbase[byI2CNumber]->F  = 0x14;       /* Chon toc do 100Khz. set MULT and ICR */
	
	/* Config SCL pin */    
	port = EXTRACT_PORT(g_i2cArray[byI2CNumber].scl);
	pin = EXTRACT_PIN(g_i2cArray[byI2CNumber].scl);

	SIM->SCGC5 |= g_portclock[port];
	g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(2);
	
	/*Config SDA pin*/
	port = EXTRACT_PORT(g_i2cArray[byI2CNumber].sda);
	pin = EXTRACT_PIN(g_i2cArray[byI2CNumber].sda);

	SIM->SCGC5 |= g_portclock[port];
	g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(2);
	
	g_i2cbase[byI2CNumber]->C1 = I2C_C1_IICEN_MASK;       /* enable I2C */
}

/*******************************************************************/
/*!
 * Start I2C Transmision
 * @param SlaveID is the 7 bit Slave Address
 * @param Mode sets Read or Write Mode
 */
void IIC_StartTransmission (u8 SLAVE_ADDR, u8 Mode)
{
	unsigned char MasterTransmission;
	unsigned char SlaveID;
  if(Mode == WRITE)
  {
    /* set transmission mode */
    MasterTransmission = WRITE;
  }
  else
  {
    /* set transmission mode */
    MasterTransmission = READ;
  }

  /* shift ID in right possition */
  SlaveID = (unsigned char) SLAVE_ADDR << 1;

  /* Set R/W bit at end of Slave Address */
  SlaveID |= (unsigned char)MasterTransmission;

  /* send start signal */
  i2c_Start();

  /* send ID with W/R bit */
  i2c_write_byte(SlaveID);
}

/*******************************************************************/
/*!
 * Pause Routine
 */
void Pause(void){
    int n;
    for(n=1;n<500;n++) {
    }
}

/*******************************************************************/
/*!
 * Write a byte of Data to specified register on MMA7660
 * @param u8RegisterAddress is Register Address
 * @param u8Data is Data to write
 */
void I2CWriteRegister(u8 SLAVE_ADDR, u8 u8RegisterAddress, u8 u8Data)
{
  /* send data to slave */
  IIC_StartTransmission(SLAVE_ADDR, WRITE);
  i2c_Wait();

  g_i2cbase[i2cNumber]->D = u8RegisterAddress;
  i2c_Wait();

  g_i2cbase[i2cNumber]->D = u8Data;
  i2c_Wait();

  i2c_Stop();

  Pause();
}

void I2CWriteMultiRegisters(u8 SLAVE_ADDR, u8 u8RegisterAddress, u8_p data, u8 bytes)
{
	int i;
	int n = bytes;
	/* send data to slave */
  IIC_StartTransmission(SLAVE_ADDR, WRITE);
  i2c_Wait();

  g_i2cbase[i2cNumber]->D = u8RegisterAddress;
  i2c_Wait();

	for(i=0; i<n; i++)
	{
		g_i2cbase[i2cNumber]->D = data[i];
		i2c_Wait();
	}
	
  i2c_Stop();

  Pause();
}

/*******************************************************************/
/*!
 * Read a register from the MMA7660
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
unsigned char I2CReadRegister(u8 SLAVE_ADDR, u8 u8RegisterAddress)
{
  unsigned char result;

  /* Send Slave Address */
  IIC_StartTransmission(SLAVE_ADDR, WRITE);
  i2c_Wait();

  /* Write Register Address */
  g_i2cbase[i2cNumber]->D = u8RegisterAddress;
  i2c_Wait();

  /* Do a repeated start */
  i2c_RepeatedStart();											//ban dau la: I2C1_C1 |= I2C_C1_RSTA_MASK;

  /* Send Slave Address */
  g_i2cbase[i2cNumber]->D = (SLAVE_ADDR << 1) | READ; //read address
  i2c_Wait();

	i2c_EnterRxMode();

  /* Dummy read */
  result = g_i2cbase[i2cNumber]->D ;
  i2c_Wait();

  /* Send stop since about to read last byte */
  i2c_Stop();

  /* Read byte */
  result = g_i2cbase[i2cNumber]->D ;

  return result;
}

/*******************************************************************/
/*!
 * Read first three registers from the MMA7660
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
void I2CReadMultiRegisters(u8 SLAVE_ADDR, u8 u8RegisterAddress, u8_p data, u8 bytes)
{
  unsigned char n=bytes;
  int i;

  /* Send Slave Address */
  IIC_StartTransmission(SLAVE_ADDR, WRITE);
  i2c_Wait();

  /* Write Register Address */
  g_i2cbase[i2cNumber]->D = u8RegisterAddress;
  i2c_Wait();

  /* Do a repeated start */
  i2c_RepeatedStart();			//I2C1_C1 |= I2C_C1_RSTA_MASK;

  /* Send Slave Address */
  g_i2cbase[i2cNumber]->D = (SLAVE_ADDR << 1) | READ; //read address
  i2c_Wait();
	
	i2c_EnterRxMode();

  /* Dummy read */
  data[0] = g_i2cbase[i2cNumber]->D ;
  i2c_Wait();

  for(i=0;i<n;i++)//i<n-2
  {
    /* Read first byte */
    data[i] = g_i2cbase[i2cNumber]->D;
    i2c_Wait();
  }
  //Gui tin hieu NACK
  g_i2cbase[i2cNumber]->C1 |= I2C_C1_TXAK_MASK;

  /* Send stop */
  i2c_Stop();
}

