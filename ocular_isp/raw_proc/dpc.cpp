
/*
* reference: robust defect pixel detection and correction for bayer imaging system
* 
* 
* 
*/


#include"dpc.h"
#include"common.h"


void get_mean(uint16_t* src, double& avg)
{
	int i;

	uint32_t sum = 0;
	bubble_sort(src, 9);

	for (i = 0; i < 9; i++)
	{
		if ((i > 0) && (i < 8))
		{
			sum += src[i];
		}
	}

	avg = sum / 7.0;

}


void condition_a(uint16_t current, double avg, float m1, bool& cold, bool& hot)
{

	if (current > ((1 + m1) * avg))
	{
		hot = true;
	}

	if (current < ((1 - m1) * avg))
	{
		cold = true;
	}

}


void condition_b(double* patch, double m2, bool& cold, bool& hot)
{
	double current = patch[4];

	double min_1 = MIN(patch[1], patch[7]);
	double min_2 = MIN(patch[3], patch[5]);
	double min_31 = MIN(patch[0], patch[2]);
	double min_32 = MIN(patch[6], patch[8]);
	double min_3 = MIN(min_31, min_32);

	double max_1 = MAX(patch[1], patch[7]);
	double max_2 = MAX(patch[3], patch[5]);
	double max_31 = MAX(patch[0], patch[2]);
	double max_32 = MAX(patch[6], patch[8]);
	double max_3 = MAX(max_31, max_32);

	if ((current > (m2 * min_1)) && (current > (m2 * min_2)) && (current > (m2 * min_3)))
		hot = true;

	if ((current < (m2 * max_1)) && (current < (m2 * max_2)) && (current < (m2 * max_3)))
		cold = true;

}



void dpc_process(uint16_t* src, int32_t height, int32_t width, COMMON_PARAMS* common_params, uint16_t* dst)
{

	int i, j, k, l;
	int ii, jj;
	int pos = 0;

	float m1 = 0.5;	// (0, 1)
	double m2 = 4.5; // [1.0, 5]

	bool cold_a = false, hot_a = false;
	bool cold_b = false, hot_b = false;
	bool cold = false, hot = false;

	int white_level = common_params->white_level;

	int patch_size = 3;
	uint16_t* patch_5x5 = new uint16_t[patch_size * patch_size];
	memset(patch_5x5, 0, patch_size * patch_size * sizeof(uint16_t));

	double* avg_map = new double[height * width];
	memset(avg_map, 0.0, height * width * sizeof(double));

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pos = i * width + j;
			get_patch(src, i, j, height, width, patch_size, 2, patch_5x5);
			get_mean(patch_5x5, avg_map[pos]);
		}
	}

	double* patch_avg = new double[patch_size * patch_size];
	memset(patch_avg, 0.0, patch_size * patch_size * sizeof(double));

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pos = i * width + j;
			condition_a(src[pos], avg_map[pos], m1, cold_a, hot_a);
			
			get_patch(avg_map, i, j, height, width, patch_size, 1, patch_avg);

			condition_b(patch_avg, m2, cold_b, hot_b);

			cold = cold_a && cold_b;
			hot = hot_a && hot_b;

			// todo: defect point correct
			//if (cold || hot)
			//{
			//	dst[pos] = 100;
			//}
			//else
			//{
				dst[pos] = src[pos];
			//}

			cold_a = false;
			hot_a = false;
			cold_b = false;
			hot_b = false;

		}
	}


	delete[] patch_5x5;
	delete[] avg_map;
	delete[] patch_avg;
}




void run_dpc(uint16_t* src, int32_t height, int32_t width, COMMON_PARAMS* common_params, uint16_t* dst)
{
	bool bypass = false;

	if (bypass)
	{
		bypass_operator(src, height, width, dst);
	}
	else
	{
		dpc_process(src, height, width, common_params, dst);
	}

}

