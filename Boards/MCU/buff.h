/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef __BUFF_H__
#define __BUFF_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "macro.h"
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MAX_BUFF_SIZE                  1024
#define MAX_BUFF_CMD                   8
#define ERR_OK                         0x00 
#define ERR_BUF_FUL                    0x01
#define ERR_BUF_EMPTY                  0x02     
#define BUFF_NOT_OVERRIDE              0
#define BUFF_OVERRIDE                  1

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typedef struct __cmd__ {
    u16_t offset;
    u16_t length;
} cmd_t, *cmd_p;

typedef struct __buff_data__ {
    u8_t pData[MAX_BUFF_SIZE];
    u16_t wCountEle;
    u8_t type;
    u16_t wfront;
    u16_t wrear;
} buffdat_t, *buffdat_p;

typedef struct __buff_cmd__ {
    buffdat_t buffdat;

    cmd_t cmd[MAX_BUFF_CMD];
    u8_t byCountCmd;
    u8_t type;
    u8_t byfront;
    u8_t byrear;
} buffCmd_t, *buffCmd_p;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
u8_t 
bufAddDat(
    u8_t byDat,
    buffdat_p buf
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
u8_t
bufDecreDat(
    u8_p pbyDat,
    buffdat_p buf    
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
u16_t 
bufNumEle(
    buffdat_p buf
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void_t
bufClear(
    buffdat_p buf
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
u8_t
deQueueCmd(
    u8_p pData,
    u8_p pLen,
    buffCmd_p bufCmd
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
u8_t
enQueueCmd(
    u8_p pData,
    u8_t len,
    buffCmd_p bufCmd
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void_t
bufCmdClear(
    buffCmd_p buf
);

#endif /* END FILE */
