#ifndef __APP_TEST_H__
#define __APP_TEST_H__

#include "typedef.h"
#include "app_adc.h"

extern UINT8 OverCurrentChannelFlag[AD_CHANNEL_NUM];
extern BOOL b_TestMode;

void app_testInit(void);
void app_testOverCurrentInit(void);
void app_testOverCurrentProtection(void);
void app_testLowCurrentCloseHandler1s(void);
void app_testOverCurrentRecoveryHandler10s(void);
void app_testCurrentTest1s(void);

#endif
