#include<iostream>

#include"common.h"
#include"conv.h"


static const RAW_PIXEL_TYPE raw_pixel_type_lut[4][4] =
{
	{R, Gr, Gb, B},
	{Gr, R, B, Gb},
	{Gb, B, R, Gr},
	{B, Gb, Gr, R}
};

RAW_PIXEL_TYPE get_raw_pixel_type(const BAYER_PATTERN pattern, const int s32x, const int s32y)
{
	int i = ((s32y & 1) << 1) + (s32x & 1);
	return raw_pixel_type_lut[pattern][i];
}


void bayer_to_gray(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{
	// r g r g
	// g b g b
	// r g r g
	// g b g b

	int i, j;

	// for r, b channel
	int kernel_1[9] = {
		1, 2, 1,
		2, 4, 2,
		1, 2, 1
	};

	// for g channel
	int kernel_2[9] = {
		2, 1, 2,
		1, 4, 1,
		2, 1, 2
	};

	conv2d(src, height, width, kernel_1, 3, 4, dst);

}



void bypass_operator(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	int i, j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			*dst = src[i * width + j];
			dst++;
		}
	}

}

