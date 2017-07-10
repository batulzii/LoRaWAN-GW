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
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "buff.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

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
) {

    if ((buf->wCountEle >= MAX_BUFF_SIZE) && (buf->type == BUFF_NOT_OVERRIDE))
        return ERR_BUF_FUL;

		if (buf->wrear > MAX_BUFF_SIZE) {
		while (1);
		}
    buf->pData[buf->wrear] = byDat;
    buf->wrear = (buf->wrear + 1) & (MAX_BUFF_SIZE - 1);

    if (MAX_BUFF_SIZE > buf->wCountEle)
        buf->wCountEle++;
    else if (BUFF_OVERRIDE == buf->type)
        buf->wfront = buf->wrear;

    return ERR_OK;
}

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
) {
    if (bufNumEle(buf) > 0) {
        *pbyDat = buf->pData[buf->wfront];  
         buf->wfront = (buf->wfront +1 ) & (MAX_BUFF_SIZE - 1);
         buf->wCountEle--; 

         return ERR_OK;
    }
    
    return ERR_BUF_EMPTY;
}

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
) {
    return buf->wCountEle;
}

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
) {
    buf->wCountEle = 0;
    buf->wfront = 0;
    buf->wrear = 0;
}

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
) {
    u16_t i;
//    u16_t offset; 
    u8_t cXOR = 0;

    if ((bufCmd->byCountCmd >= MAX_BUFF_CMD) && (bufCmd->type == BUFF_NOT_OVERRIDE))
        return ERR_BUF_FUL;

    bufCmd->cmd[bufCmd->byrear].length = len + 1;
    bufCmd->cmd[bufCmd->byrear].offset = bufCmd->buffdat.wrear;

    for (i = 0; i < len; i++) {
        bufAddDat(pData[i], &bufCmd->buffdat);
        cXOR ^= pData[i];
    } 
       
    bufAddDat(cXOR, &bufCmd->buffdat);
    
    bufCmd->byrear = (bufCmd->byrear + 1) & (MAX_BUFF_CMD - 1);

    if (MAX_BUFF_CMD > bufCmd->byCountCmd)
        bufCmd->byCountCmd++;
    else if (bufCmd->type == BUFF_OVERRIDE) {
        bufCmd->byfront = bufCmd->byrear;   
        bufCmd->buffdat.wfront = bufCmd->cmd[bufCmd->byfront].offset; 
    }
    return ERR_OK;
}

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
) {
    u16_t i;
    u8_t byTemp;
    
    if (bufCmd->byCountCmd > 0) {
        *pLen = bufCmd->cmd[bufCmd->byfront].length; 
        for (i = 0; i < *pLen; i++) {
             bufDecreDat(&byTemp, &bufCmd->buffdat);
             pData[i] = byTemp;
        }
 
        bufCmd->byfront = (bufCmd->byfront +1 ) & (MAX_BUFF_CMD - 1);
        bufCmd->byCountCmd--;  
    
        return ERR_OK;
    }

    return ERR_BUF_EMPTY;
}

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
) {
    buf->byCountCmd = 0;
    buf->byfront = 0;
    buf->byrear = 0;
    buf->type = BUFF_NOT_OVERRIDE;
}
/* END FILE */
