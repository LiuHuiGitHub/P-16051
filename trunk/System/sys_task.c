#include "sys.h"
#include "stc15f2k60s2.h"
#include "drive_buzzer.h"
#include "drive_relay.h"
#include "drive_led.h"
#include "sys_uart.h"
#include "app_brush.h"
#include "app_adc.h"
#include "app_config.h"
#include "app_time.h"
#include "app_test.h"
#include "stdio.h"

//UINT8 lastChannel = 0;
//UINT8 buff[30];
//void sys_taskHandler1s(void)
//{
//	if(app_adcCycleSamplingChannel() != lastChannel)
//	{
//        if(lastChannel != AD_CHANNEL_NUM)
//        {
//            sprintf(buff, "ch%c:%5.3fA,time:%3d,%1d\n", lastChannel+'0', app_adcGetValue(lastChannel)*5.0/1.245/1024, u16_DisplayTime[lastChannel],(UINT16)OverCurrentChannelFlag[lastChannel]);
//            sys_uartSendString(buff);
//        }
//        lastChannel = app_adcCycleSamplingChannel();
//	}
//}

void sys_Init(void)
{
	drv_buzzerInit();
	drv_relayInit();
	drv_ledInit();
	sys_uartInit();
    app_adcInit();
	app_brushInit();
	app_configInit();
	app_timeInit();
	drv_buzzerNumber(1);
    app_testOverCurrentInit();
}

void sys_taskAdd(void)
{
	sys_taskCreate(app_adcHandler1ms, 1, TASK_HANDLER);
	sys_taskCreate(drv_ledHandler1ms, 1, TASK_HANDLER);
	sys_taskCreate(drv_buzzerHandler10ms, 11, TASK_HANDLER);
	sys_taskCreate(app_brushCycle500ms, 501, TASK_CYCLE);
	sys_taskCreate(app_timeDisplay500ms, 499, TASK_CYCLE);
    sys_taskCreate(app_testOverCurrentProtection, 1000, TASK_CYCLE);
    sys_taskCreate(app_testOverCurrentRecoveryHandler10s, 10000, TASK_CYCLE);
	sys_taskCreate(app_timePower1min, 60000, TASK_CYCLE);
	sys_taskCreate(app_testCurrentTest1s, 1000, TASK_CYCLE);
	sys_taskCreate(app_testLowCurrentCloseHandler1s, 1000, TASK_CYCLE);
	sys_taskCreate(app_Show, 0, TASK_ONCE);
}
