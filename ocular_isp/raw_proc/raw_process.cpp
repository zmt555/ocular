#include"edge_filters.h"
#include"common.h"
#include"raw_params.h"
#include"raw_process.h"

#include<iostream>
#include<string>


using namespace std;



void run_process(uint16_t* src, int32_t height, int32_t width, COMMON_PARAMS* common_params, BLC_PARAMS* blc_params, AWB_PARAMS *awb_params, uint8_t* dst)
{
	int i, j;

	uint16_t* raw_temp = new uint16_t[height * width];
	memset(raw_temp, 0, height * width * sizeof(uint16_t));

	// raw process
	// dpc
	run_dpc(src, height, width, raw_temp);

	// raw nr
	run_rawnr(raw_temp, height, width, raw_temp);

	// blc
	run_blc(raw_temp, height, width, raw_temp);

	// lsc
	run_lsc(raw_temp, height, width, raw_temp);

	// wbc
	run_wbc(raw_temp, height, width, raw_temp);

	// tone mapping
	run_gtm(raw_temp, height, width, raw_temp);

	run_ltm(raw_temp, height, width, raw_temp);

	// cac
	run_cac(raw_temp, height, width, raw_temp);

	// demosaicing
	run_demosaic(raw_temp, height, width, dst);

	delete[] raw_temp;

}


void raw_proc(uint16_t* src, string params_path,  int32_t height, int32_t width, uint8_t* dst)
{
	// get params

	COMMON_PARAMS* common_params = (COMMON_PARAMS*)malloc(sizeof(COMMON_PARAMS));
	BLC_PARAMS* blc_params = (BLC_PARAMS*)malloc(sizeof(BLC_PARAMS));
	AWB_PARAMS* awb_params = (AWB_PARAMS*)malloc(sizeof(AWB_PARAMS));

	read_params(params_path, common_params, blc_params, awb_params);

	// raw domain processing
	run_process(src, height, width, common_params, blc_params, awb_params, dst);

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


