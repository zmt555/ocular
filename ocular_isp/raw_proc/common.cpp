#include<iostream>

#include"conv.h"


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
