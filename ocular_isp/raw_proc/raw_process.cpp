#include"edge_filters.h"
#include"common.h"

#include<iostream>




void raw_proc(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{

}


void test_function(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{
	int16_t* gray = new int16_t[height * width];
	memset(gray, 0, width * height * sizeof(int16_t));

	bayer_to_gray(src, height, width, gray);

	// test filters
	// roberts_operator(gray, height, width, dst);
	// prewitt_operator(gray, height, width, dst);
	// sobel_operator(gray, height, width, dst);
	// scharr_operator(gray, height, width, dst);
	// laplace_operator(gray, height, width, dst);
	box_filter(gray, height, width, dst);


	delete[] gray;
}


