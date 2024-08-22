#include"wbc.h"
#include"common.h"



void wbc_process(uint16_t* src, int32_t height, int32_t width, AWB_PARAMS* awb_params, COMMON_PARAMS* common_params, uint16_t* dst)
{

	int i, j;
	int pos;

	RAW_PIXEL_TYPE pix_type;

	float r_gain = awb_params->r_gain;
	float g_gain = awb_params->g_gain;
	float b_gain = awb_params->b_gain;

	int white_level = common_params->white_level;
	BAYER_PATTERN pattern = (BAYER_PATTERN)common_params->bayer_pattern;


	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pos = i * width + j;

			pix_type = get_raw_pixel_type(pattern, j, i);

			switch (pix_type)
			{
			case R:
				dst[pos] = uint16_t(CLIP((src[pos] * r_gain), 0, white_level));
				break;
			case Gr:
			case Gb:
				dst[pos] = uint16_t(CLIP((src[pos] * g_gain), 0, white_level));
				break;
			case B:
				dst[pos] = uint16_t(CLIP((src[pos] * b_gain), 0, white_level));
				break;
			}
		}
	}

}




void run_wbc(uint16_t* src, int32_t height, int32_t width, AWB_PARAMS* awb_params, COMMON_PARAMS* common_params, uint16_t* dst)
{
	bool bypass = false;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		wbc_process(src, height, width, awb_params, common_params, dst);
	}

}
