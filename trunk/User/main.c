#include "sys.h"
#include "sys_task.h"

void main(void)
{
	sys_Init();
    sys_taskInit();
    sys_taskAdd();
	sys_taskStart();
}

#ifdef USE_FULL_ASSERT
void assert_failed(SYS_UINT8* file, SYS_UINT32 line)
{
	while(1);
}
#endif
