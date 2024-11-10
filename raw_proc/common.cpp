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


void bypass_operator(uint16_t* src, int32_t height, int32_t width, uint16_t* dst, int shift)
{
	int i, j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			*dst = src[i * width + j] >> shift;
			dst++;
		}
	}

}


int mirror(int a, int b, int c)
{
	a = (a > b) ? a : (b - a);
	a = (a < c) ? a : (c - (a - c));

	return a;
}



void get_patch(uint16_t* src, int y, int x, int32_t height, int32_t width, int patchsize, int stride, uint16_t* dst)
{
	int i, j;
	int ii, jj;

	int half_size = patchsize >> 1;

	for (i = -half_size; i <= half_size; i++)
	{
		for (j = -half_size; j <= half_size; j++)
		{
			ii = y + i * stride;
			ii = mirror(ii, 0, height - 1);

			jj = x + j * stride;
			jj = mirror(jj, 0, width - 1);

			*dst = src[ii * width + jj];
			dst++;
		}
	}
}


void get_patch(double* src, int y, int x, int32_t height, int32_t width, int patchsize, int stride, double* dst)
{
	int i, j;
	int ii, jj;

	int half_size = patchsize >> 1;

	for (i = -half_size; i <= half_size; i++)
	{
		for (j = -half_size; j <= half_size; j++)
		{
			ii = y + i * stride;
			ii = mirror(ii, 0, height - 1);

			jj = x + j * stride;
			jj = mirror(jj, 0, width - 1);

			*dst = src[ii * width + jj];
			dst++;
		}
	}
}


void swap(uint16_t* src, int i, int j)
{
	uint16_t temp = src[i];
	src[i] = src[j];
	src[j] = temp;
}


void bubble_sort(uint16_t* src, int nums)
{
	int i, j;
	for (i = 0; i < nums - 1; i++)
	{
		for (j = i + 1; j <= nums; j++)
		{
			if (src[i] > src[j])
			{
				swap(src, i, j);
			}
		}
	}
}
