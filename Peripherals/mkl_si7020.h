#ifndef _MKL_SI7020_H_
#define _MKL_SI7020_H_

#include "typedefs.h"
#include "i2c.h"

#define SI7020_ADDR    	(0x40)  // 1000000x

/***** File Scope Data *****/
static const char cmd_meas_humid[]         = 	{ 0xE5 };
static const char cmd_meas_humid_no_hold[] = 	{ 0xF5 };
static const char cmd_meas_temp[]          = 	{ 0xE3 };
static const char cmd_meas_temp_no_hold[]  = 	{ 0xF3 };
static const char cmd_meas_prev_temp[]     =  { 0xE0 };
static const char cmd_rst[]                = 	{ 0xFE };
static const char cmd_write_user1[]        = 	{ 0xE6 };
static const char cmd_read_user1[]         = 	{ 0xE7 };
static const char cmd_id_1[]               = 	{ 0xFA, 0x0F };
static const char cmd_id_2[]               = 	{ 0xFC, 0xC9 };
static const char cmd_fw_ver[]             = 	{ 0x84, 0xB8 };

typedef enum {
        RH_12b_TEMP_14b = 0x0, ///< 12 bits for RH, 14 bits for Temp
        RH_8b_TEMP_12b  = 0x1,  ///< 8 bits for RH, 12 bits for Temp
        RH_10b_TEMP_13b = 0x2, ///< 10 bits for RH, 13 bits for Temp
        RH_11b_TEMP_11b = 0x3, ///< 11 bits for RH, 11 bits for Temp
} resolution_t;

u8 si7020_temperature(void);
u8 si7020_humi(void);

#endif
