/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: TrungTQb
 *
 * Last Changed By:  $Author: trungtq $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-16 11:45:00 (Tue, 16 May 2016) $
 *
 ******************************************************************************/
#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef unsigned char           byte_t;   // 1 byte
typedef unsigned short          word_t;   // 2 byte
typedef unsigned long           dword_t;  // 4 byte
typedef unsigned int            uint_t;   // 4 byte
typedef signed int              int_t;
typedef unsigned char           bool_t;
typedef char                    char_t;   // 1 byte
typedef void                    void_t;

typedef byte_t*                 byte_p;
typedef word_t*                 word_p;
typedef dword_t*                dword_p;
typedef uint_t*                 uint_p;
typedef char_t*                 char_p;
typedef void*                 	void_p;

typedef void                        (*VOID_CALLBACKFUNC)(void);
typedef void                        (*BYTE_CALLBACKFUNC)(byte_t);
typedef void                        (*WORD_CALLBACKFUNC)(word_t);

typedef unsigned char           u8;       // 1 byte
typedef unsigned short          u16;      // 2 byte
typedef unsigned long           u32;      // 4 byte
typedef unsigned long long      u64;

typedef unsigned char           u8_t;     // 1 byte
typedef unsigned short          u16_t;    // 2 byte
typedef unsigned long           u32_t;    // 4 byte
typedef unsigned long long      u64_t;

typedef signed char             i8;       // 1 byte
typedef signed short            i16;      // 2 byte
typedef signed long             i32;      // 4 byte
typedef signed long long        i64;

typedef unsigned char*          u8_p;     // 1 byte
typedef unsigned short*         u16_p;    // 2 byte
typedef unsigned long*          u32_p;    // 4 byte
typedef unsigned long long*     u64_p; 

typedef signed char*            i8_p;     // 1 byte
typedef signed short*           i16_p;    // 2 byte
typedef signed long*            i32_p;    // 4 byte
typedef signed long long*       i64_p;

#ifndef TRUE
#define FALSE                   0
#define TRUE                    1
#endif // TRUE

typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {
    RESET = 0, 
    SET = !RESET
} FlagStatus, ITStatus;

typedef enum {
    LOW,
		HIGH,
		TOGGLE
} StatusPeripheral;

#ifndef READ
#define READ										TRUE
#define WRITE										FALSE
#endif	//READ

#ifndef NULL
#define NULL                    (0)
#endif // NULL

#ifndef MAX_U8
#define MAX_U8                  0xFF
#define MAX_U16                 0xFFFF
#define MAX_U32                 0xFFFFFFFF
#endif // MAX_U8

#define GET_BIT_0_7(value)         ((u8_t)((value) & 0xFFU))
#define GET_BIT_8_15(value)        ((u8_t)(((value)>>8) & 0xFFU))
#define GET_BIT_16_23(value)       ((u8_t)(((value)>>16) & 0xFFU))
#define GET_BIT_24_31(value)       ((u8_t)((value)>>24))

#define GET_BIT_0_15(value)        ((u16_t)((value) & 0xFFFFU))
#define GET_BIT_16_31(value)       ((u16_t)(((value)>>16) & 0xFFFFU))

#define JOIN_BIT_U16(x,y)          ((u32_t)((x<<16) + (y)))
#define JOIN_BIT_U8(x,y,z,t)       ((u32_t)((x<<24) + (y<<16) + (z<<8) + (t)))

#define READ32(address)                 ((u32_t)(*(u32_t*)(address)))
#define READ16(address)                 ((u16_t)(*(u16_t*)(address)))
#define READ8(address)                  ((u8_t)(*(u8_t*)(address)))
#define REG_WRITE(address, value)       (*(vuint8_t*)(address) = (value))

#ifndef BIT
#define BIT(x) (1U << (x))  // Unsigned avoids compiler warnings re BIT(15)
#endif



typedef void (*timer_callback)(void_p);
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

#endif /* _TYPEDEFS_H_ */
