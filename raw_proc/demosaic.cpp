#include"demosaic.h"
#include"common.h"
#include<opencv.hpp>

using namespace cv;

void dmc_process(uint16_t* src, int32_t height, int32_t width, COMMON_PARAMS* common_params, uint8_t* dst)
{
	int i, j;
	int pos = 0;

	BAYER_PATTERN pattern = (BAYER_PATTERN)common_params->bayer_pattern;

	uint16_t* src_temp = new uint16_t[height * width];
	memset(src_temp, 0, height * width * sizeof(uint16_t));

	Mat bayer = Mat::zeros(Size(width, height), CV_8UC1);
	Mat bgr_map = Mat::zeros(Size(width,height), CV_8UC3);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			bayer.at<uint8_t>(i, j) = src[i * width + j] >> 4;
		}
	}
	
	//namedWindow("bayer", WINDOW_FREERATIO);
	//imshow("bayer", bayer);
	//waitKey(0);

	// demosaicing(bayer, bgr_map, COLOR_BayerRGGB2BGR);
	demosaicing(bayer, bgr_map, COLOR_BayerGBRG2BGR);
	
	switch (pattern)
	{
	case RGGB:
		demosaicing(bayer, bgr_map, COLOR_BayerRGGB2BGR);
		break;

	case GRBG:
		demosaicing(bayer, bgr_map, COLOR_BayerGRBG2BGR);
		break;

	case GBRG:
		demosaicing(bayer, bgr_map, COLOR_BayerGBRG2BGR);
		break;

	case BGGR:
		demosaicing(bayer, bgr_map, COLOR_BayerBGGR2BGR);
		break;
	}
	
	uint8_t *temp_ptr = bgr_map.ptr<uint8_t>(0);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			*dst++ = *temp_ptr++;
			*dst++ = *temp_ptr++;
			*dst++ = *temp_ptr++;
		}
	}

	delete[] src_temp;
}




void run_demosaic(uint16_t* src, int32_t height, int32_t width, COMMON_PARAMS* common_params, uint8_t* dst)
{
	bool bypass = false;

	dmc_process(src, height, width, common_params, dst);

}
