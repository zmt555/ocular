#include"rgb_process.h"



void rgb_process(uint8_t* src, int32_t height, int32_t width, uint8_t* dst)
{
	uint8_t* rgb_temp = new uint8_t[3 * height * width];
	memset(rgb_temp, 0, height * width * 3 * sizeof(uint8_t));


	run_gamma(src, height, width, dst);


	delete[] rgb_temp;


}


void rgb_proc(uint8_t* src, int32_t height, int32_t width, uint8_t* dst)
{
	rgb_process(src, height, width, dst);
}


