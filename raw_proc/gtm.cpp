#include"gtm.h"
#include"common.h"



void gtm_process(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{





}




void run_gtm(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	bool bypass = true;

	if (bypass)
	{
		bypass_operator(src, height, width, dst, 0);
	}
	else
	{
		gtm_process(src, height, width, dst);
	}

}
