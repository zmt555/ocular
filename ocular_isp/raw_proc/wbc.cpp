#include"wbc.h"
#include"common.h"



void wbc_process(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{





}




void run_wbc(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	bool bypass = true;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		wbc_process(src, height, width, dst);
	}

}
