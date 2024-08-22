#include"blc.h"
#include"common.h"



void blc_process(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	RAW_PIXEL_TYPE pix_type;




}




void run_blc(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	bool bypass = true;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		blc_process(src, height, width, dst);
	}

}
