#include"blc.h"
#include"common.h"



void blc_process(uint16_t* src, int32_t height, int32_t width, BLC_PARAMS* blc_params, COMMON_PARAMS* common_params, uint16_t* dst)
{
	int i, j;
	int pos = 0;

	RAW_PIXEL_TYPE pix_type;

	int blc_r = blc_params->black_level_r;
	int blc_gr = blc_params->black_level_gr;
	int blc_gb = blc_params->black_level_gb;
	int blc_b = blc_params->black_level_b;

	int white_level = common_params->white_level;
	BAYER_PATTERN pattern = (BAYER_PATTERN)common_params->bayer_pattern;

	float coef_r = white_level / (white_level - blc_r);
	float coef_gr = white_level / (white_level - blc_gr);
	float coef_gb = white_level / (white_level - blc_gb);
	float coef_b = white_level / (white_level - blc_b);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pos = i * width + j;

			pix_type = get_raw_pixel_type(pattern, j, i);

			switch (pix_type)
			{
			case R:
				dst[pos] = uint16_t((src[pos] - blc_r) * coef_r);
				break;
			case Gr:
				dst[pos] = uint16_t((src[pos] - blc_gr) * coef_gr);
				break;
			case Gb:
				dst[pos] = uint16_t((src[pos] - blc_gb) * coef_gb);
				break;
			case B:
				dst[pos] = uint16_t((src[pos] - blc_b) * coef_b);
				break;
			}
		}
	}
}




void run_blc(uint16_t* src, int32_t height, int32_t width, BLC_PARAMS* blc_params, COMMON_PARAMS* common_params, uint16_t* dst)
{
	bool bypass = false;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		blc_process(src, height, width, blc_params, common_params, dst);
	}

}
