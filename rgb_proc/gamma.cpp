#include"gamma.h"


uint8_t gamma_lut[256];


void build_lut(double gamma)
{
	int i;
	double f;

	gamma = 1.0 / gamma;

	for (i = 0; i < 256; i++)
	{
		f = (i + 0.5f) / 256;
		f = (double)pow(f, gamma);

		gamma_lut[i] = (uint8_t)(f * 256 - 0.5f);

	}

}


void gamma_process(uint8_t* src, int32_t height, int32_t width, uint8_t* dst)
{
	int i, j, k;

	int pos = 0;


	build_lut(2.2);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			*dst++ = gamma_lut[*src++];
			*dst++ = gamma_lut[*src++];
			*dst++ = gamma_lut[*src++];
		}
	}
}




void run_gamma(uint8_t* src, int32_t height, int32_t width, uint8_t* dst)
{
	gamma_process(src, height, width, dst);
}
