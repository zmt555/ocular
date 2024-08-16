#include<iostream>

using namespace std;


typedef struct _ocular_isp_common_param_t {

	int bayer_pattern; // 0 rggb 1 grbg 2 gbrg 3 bggr
	int white_level;
	int img_height;
	int img_width;
	int bitwidth;

}COMMON_PARAMS;


typedef struct _ocular_isp_blc_param_t {

	int black_level_r;
	int black_level_gr;
	int black_level_gb;
	int black_level_b;
}BLC_PARAMS;


typedef struct _ocular_isp_awb_param_t {

	float r_gain;
	float g_gain;
	float b_gain;

}AWB_PARAMS;


void read_params(string path, COMMON_PARAMS* common_params, BLC_PARAMS* blc_params, AWB_PARAMS* awb_params);


