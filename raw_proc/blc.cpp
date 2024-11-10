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

	int16_t temp_val = 0;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pos = i * width + j;

			pix_type = get_raw_pixel_type(pattern, j, i);

			temp_val = (int16_t)src[pos];

			switch (pix_type)
			{
			case R:
				temp_val = (temp_val - blc_r) * coef_r;
				temp_val = temp_val < 0 ? 0 : temp_val;
				temp_val = temp_val > white_level ? white_level : temp_val;
				dst[pos] = uint16_t(temp_val);
				break;
			case Gr:
				temp_val = (temp_val - blc_gr) * coef_gr;
				temp_val = temp_val < 0 ? 0 : temp_val;
				temp_val = temp_val > white_level ? white_level : temp_val;
				dst[pos] = uint16_t(temp_val);
				break;
			case Gb:
				temp_val = (temp_val - blc_gb) * coef_gb;
				temp_val = temp_val < 0 ? 0 : temp_val;
				temp_val = temp_val > white_level ? white_level : temp_val;
				dst[pos] = uint16_t(temp_val);
				break;
			case B:
				temp_val = (temp_val - blc_b) * coef_b;
				temp_val = temp_val < 0 ? 0 : temp_val;
				temp_val = temp_val > white_level ? white_level : temp_val;
				dst[pos] = uint16_t(temp_val);
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
		bypass_operator(src, height, width, dst, 0);
	}
	else
	{
		blc_process(src, height, width, blc_params, common_params, dst);
	}

}