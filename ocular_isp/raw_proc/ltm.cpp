#include"ltm.h"
#include"common.h"



void ltm_process(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{





}




void run_ltm(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	bool bypass = true;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		ltm_process(src, height, width, dst);
	}

}
