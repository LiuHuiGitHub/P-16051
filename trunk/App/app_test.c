#include "STC15F2K60S2.h"
#include "app_test.h"
#include "app_time.h"
#include "app_config.h"
#include "app_adc.h"
#include "sys_delay.h"
#include "hwa_eeprom.h"
#include "drive_led.h"
#include "string.h"
#include "stdio.h"


UINT16 u16_LowCurrentCount[AD_CHANNEL_NUM] = {0};
#define OVER_CURRENT_TIME       	200     //100ms*200 = 20s
#define OVER_CURRENT_COUNT      	4	//过流保护动作阈值

UINT8 OverCurrentChannel = 0;           //过流保护循环检测通道
UINT8 OverCurrentChannelFlag[AD_CHANNEL_NUM];       //过流保护标志
UINT8 OverCurrentCount = 0;                 //过流保护检测累加次数
UINT8 OverCurrentTime = 0;                  //过流保护检测累加时间

BOOL b_TestMode = FALSE;

void app_testOverCurrentInit(void)
{
    memset(OverCurrentChannelFlag, FALSE, sizeof(OverCurrentChannelFlag));
}

#define OVER_STATE_NONE             0x00
#define OVER_STATE_SET_CHANNEL      0x01
#define OVER_STATE_READ_VALUE       0x02

UINT8 OverCurrentTestState = OVER_STATE_NONE;
UINT16 OverCurrentValue;                //过流保护阈值
//UINT16 u16_lastADValue[AD_CHANNEL_NUM];


void app_testLowCurrentCloseHandler1s(void)
{
    UINT8 channel;
    for(channel=0; channel<AD_CHANNEL_NUM; channel++)
    {
        if(u16_DisplayTime[channel])
        {
            if(app_adcGetValue(channel) < 30)               //低电流门限		0~1000 ->0~4A  30约110mA
            {
                u16_LowCurrentCount[channel]++;
                if(u16_LowCurrentCount[channel] > 300)        //60->1分钟
                {
            		app_timeClear(channel);
                }
            }
            else
            {
                u16_LowCurrentCount[channel] = 0;
            }
        }
        else
        {
            u16_LowCurrentCount[channel] = 0;
        }
    }
}
void app_testOverCurrentProtection(void)
{
    UINT16 NowADvalue;                      //AD检测单通道AD值
    OverCurrentValue = s_System.IMaxValue * 100; //0~9 -> 0~4A

    OverCurrentChannel %= AD_CHANNEL_NUM;
    
    switch(OverCurrentTestState)
    {
        case OVER_STATE_NONE:
        	if(u16_DisplayTime[OverCurrentChannel] == 0 ||		//无电量或通道过流时不再检测过流
        		OverCurrentChannelFlag[OverCurrentChannel])
		    {
				OverCurrentChannel++;
				OverCurrentTime = 0;
				OverCurrentCount = 0;
				return;
		    }
            NowADvalue = app_adcGetValue(OverCurrentChannel);
            if(NowADvalue > OverCurrentValue)
            {
                OverCurrentTestState = OVER_STATE_SET_CHANNEL;
                OverCurrentCount = 0;
                OverCurrentTime = OVER_CURRENT_TIME;
            }
            else
            {
                OverCurrentChannel++;
            }
            break;

        case OVER_STATE_SET_CHANNEL:
            app_adcOnceSetChannel(OverCurrentChannel);
            OverCurrentTestState = OVER_STATE_READ_VALUE;
            break;

        case OVER_STATE_READ_VALUE:
            NowADvalue = app_adcGetOnceChannelValue();
            if(NowADvalue != ADC_ONCEING)
            {
                if(NowADvalue > OverCurrentValue)
                {
                    OverCurrentCount++;
                    if(OverCurrentCount > OVER_CURRENT_COUNT)
                    {
                		OverCurrentChannelFlag[OverCurrentChannel] = TRUE;
//                    	app_timeClear(OverCurrentChannel);
			            OverCurrentTestState = OVER_STATE_NONE;
			            OverCurrentTime = 0;
			            OverCurrentChannel++;
                    }
                    else
                    {
                    	OverCurrentTestState = OVER_STATE_SET_CHANNEL;
                    }
                }
            }
            break;

        default:
            OverCurrentTestState = OVER_STATE_NONE;
            break;
    }

    if(OverCurrentTime)
    {
        OverCurrentTime--;
        if(OverCurrentTime == 0)
        {
            if(OverCurrentCount > OVER_CURRENT_COUNT)
            {
                OverCurrentChannelFlag[OverCurrentChannel] = TRUE;
            }
            else
            {
                OverCurrentChannelFlag[OverCurrentChannel] = FALSE;
            }
            OverCurrentTestState = OVER_STATE_NONE;
            OverCurrentChannel++;
        }
    }
}

void app_testOverCurrentRecoveryHandler10s(void)      //过流状态恢复
{
    UINT8 channel;
    for(channel=0; channel<AD_CHANNEL_NUM; channel++)
    {
        if(OverCurrentChannelFlag[channel])
        {
            if(app_adcGetValue(channel)<OverCurrentValue)
            {
                OverCurrentChannelFlag[channel] = FALSE;
            }
        }
    }
}

void app_testCurrentTest1s(void)
{
    if(b_TestMode == TRUE)
    {
        drv_ledRequestDisplayChannel0((UINT16)(app_adcGetValue(0)*2/5), 2000, BIT0);
        drv_ledRequestDisplayChannel1((UINT16)(app_adcGetValue(1)*2/5), 2000, BIT3);
    }
}
