#include <STC15F2K60S2.H>
#include "drive_key.h"
#include "sys_delay.h"


BOOL b_key0FaultFlag = FALSE;
BOOL b_key1FaultFlag = FALSE;
void drive_keyFaultTest10ms(void)
{
    static UINT16 key0Count = 0;
    static UINT16 key1Count = 0;
    #define KEY_FAULT_TIME      500     //5s
    if(P30 == 0)
    {
        if(key0Count < KEY_FAULT_TIME)
        {
            key0Count++;
        }
        else
        {
            b_key0FaultFlag = TRUE;
        }
    }
    else
    {
        key0Count = 0;
        b_key0FaultFlag = FALSE;
    }
    
    if(P31 == 0)
    {
        if(key1Count < KEY_FAULT_TIME)
        {
            key1Count++;
        }
        else
        {
            b_key1FaultFlag = TRUE;
        }
    }
    else
    {
        key1Count = 0;
        b_key1FaultFlag = FALSE;
    }
}

UINT8 drive_keyGetKey(void)
{
	UINT8 channel;
	if (P30 == 0 && b_key0FaultFlag==FALSE)
	{
        sys_delayms(10);
        if(P30 == 0)
        {
            channel = 0;
        }
	}
	else if (P31 == 0 && b_key1FaultFlag==FALSE)
	{
        sys_delayms(10);
        if(P31 == 0)
        {
            channel = 1;
        }
	}
	else
	{
		channel = KEY_NUMBER;
	}
	return channel;
}
