#include"edge_filters.h"
#include"common.h"
#include"raw_params.h"

#include<iostream>
#include<string>


using namespace std;


void raw_proc(int16_t* src, string params_path,  int32_t height, int32_t width, int16_t* dst)
{
	// get params

	COMMON_PARAMS* common_params = (COMMON_PARAMS*)malloc(sizeof(COMMON_PARAMS));
	BLC_PARAMS* blc_params = (BLC_PARAMS*)malloc(sizeof(BLC_PARAMS));
	AWB_PARAMS* awb_params = (AWB_PARAMS*)malloc(sizeof(AWB_PARAMS));

	read_params(params_path, common_params, blc_params, awb_params);

	// raw domain processing
	printf("1\n");

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


