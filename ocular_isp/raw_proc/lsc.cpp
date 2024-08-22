#include"lsc.h"
#include"common.h"



void lsc_process(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{





}




void run_lsc(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	bool bypass = true;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		lsc_process(src, height, width, dst);
	}

}
