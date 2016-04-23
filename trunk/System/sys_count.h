/******************************************************************************
  Copyright (C), 2001-2011, DCN Co., Ltd.
 ******************************************************************************
  File Name     : sys_count.h
  Version       : Initial Draft
  Author        : hui.liu
  Created       : 2015/1/24
  Last Modified :
  Description   : sys_count.c header file
  Function List :
  History       :
  1.Date        : 2015/1/24
    Author      : hui.liu
    Modification: Created file
******************************************************************************/

#ifndef __SYS_COUNT_H__
#define __SYS_COUNT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "typedef.h"

#define COUNT_OVER_TIME									(1000L)
#define SYS_COUNT_CHANNEL_NUM           (1u)

#include "sys.h"
#define GET_TIME_CNT										sys_timeTick


	extern void sys_countInit(void);
	extern void sys_countResetChannel(UINT8 channel);
	extern UINT32 sys_countGetTime(UINT8 channel);
	extern void sys_countAddCnt(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SYS_COUNT_H__ */
