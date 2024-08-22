#include"dpc.h"
#include"common.h"



void dpc_process(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{

}




void run_dpc(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	bool bypass = true;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		dpc_process(src, height, width, dst);
	}

}

