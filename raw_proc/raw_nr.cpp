#include"raw_nr.h"
#include"common.h"
#include"bilateral_filter.h"


void rawnr_process(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{





}




void run_rawnr(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	bool bypass = true;

	if (bypass)
	{
		bypass_operator(src, height, width, dst, 0);
	}
	else
	{
		rawnr_process(src, height, width, dst);
	}

}
