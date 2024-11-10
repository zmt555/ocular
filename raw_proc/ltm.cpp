

#include<iostream>
#include<stdlib.h>
#include"ltm.h"
#include"common.h"


void gen_grid_params(int height, int width, int grid_size_h, int grid_size_w, int grid_height, int grid_width, int* hor_start, int* hor_end, int* ver_start, int* ver_end, int* hor_center, int* ver_center)
{

	int i;

	int horizontal_remain = width - grid_width * grid_size_w;
	int vertical_remain = height - grid_height * grid_size_h;

	//std::cout << "grid_height: " << grid_height <<  std::endl;
	//std::cout << "grid_width:" << grid_width << std::endl;
	//std::cout << "horizontal_remain:" << horizontal_remain << std::endl;
	//std::cout << "vertical_remain:" << vertical_remain << std::endl;

	// horizontal start & end
	// horizontal center
	for (i = 0; i < grid_size_w; i++)
	{
		if (i == grid_size_w - 1)
		{
			hor_end[grid_size_w - 1] = width;
		}
		else
		{
			hor_end[i] = (i + 1) * grid_width;
		}
		hor_center[i] = (i + 1) * (grid_width >> 1);
		hor_start[i] = i * grid_width;
	}

	// vertical start & end
	// vertical center
	for (i = 0; i < grid_size_h; i++)
	{
		if (i == grid_size_h - 1)
		{
			ver_end[grid_size_h - 1] = height;
		}
		else
		{
			ver_end[i] = (i + 1) * grid_height;
		}
		ver_center[i] = (i + 1) * (grid_height >> 1);
		ver_start[i] = i * grid_height;
	}
}

void gen_hist(uint16_t* src, int height, int width, int ver_sta, int ver_end, int hor_sta, int hor_end, int32_t* hist)
{
	int i, j;
	int pos = 0;

	int32_t temp_val = 0;
	int32_t idx = 0;

	for (i = ver_sta; i < ver_end; i++)
	{
		for (j = hor_sta; j < hor_end; j++)
		{
			pos = i * width + j;
			temp_val = src[pos];
			idx = temp_val >> 8;
			hist[idx] += 1;
		}
	}
}

void hist_cdf(int32_t* hist, int points, int32_t* cdf)
{
	int i;
	int32_t sum = 0;
	int32_t max_val = 0;

	int32_t cdf_temp = 0;

	cdf[0] = 0;

	for (i = 0; i < points; i++)
	{
		sum = sum + hist[i];
		cdf[i + 1] = sum;
	}

	for (i = 0; i < (points + 1); i++)
	{
		cdf_temp = (cdf[i] << 12) / sum;

		cdf_temp = cdf_temp > 4095 ? 4095 : cdf_temp;

		cdf[i] = cdf_temp;
	}
}



void hist_max_and_min_limit(int32_t* hist, int points, int max_k, int max_b, int min_k, int min_b, int32_t* hist_out)
{
	int i;

	int32_t val_max, val_min;
	int32_t val_tmp;

	for (i = 0; i < points; i++)
	{
		val_max = max_b - ((i * max_k + (1 << 4)) >> 5);
		val_min = min_b - ((i * min_k + (1 << 4)) >> 5);

		val_tmp = hist[i];

		if (val_tmp > val_max)
		{
			val_tmp = val_max;
		}
		if (val_tmp < val_min)
		{
			val_tmp = val_min;
		}

		hist_out[i] = val_tmp;
	}
}

int32_t linear_interp(int x1, int x2, int y1, int y2, int x)
{
	int delta = x - x1;
	int distance = x2 - x1;

	int32_t res, res1, res2;

	res1 = delta * (y2 - y1);
	res2 = res1 / distance;
	res = res2 + y1;

	return res;

}

void bilinear_interp(uint16_t* src, int32_t* tone_curve, int height, int width, int grid_size_h, int grid_size_w, int grid_height, int grid_width, int* hor_cen, int* ver_cen, uint16_t* dst)
{
	int i, j;

	int pos;
	int curve_pos0;
	int curve_pos1;
	int curve_pos2;
	int curve_pos3;

	int32_t temp_val = 0;
	int32_t mapped_val0 = 0;
	int32_t mapped_val1 = 0;
	int32_t mapped_val = 0;

	int idx;

	int x = 0, y = 0;	// used to locate current block
	int m = 0, n = 0;	// used to locate reference points

	int32_t curve_val0, curve_val1;
	int32_t curve_delta;

	int32_t val0, val1, val2, val3;

	for (i = 0; i < height; i++)
	{
		if (i > (y + 1) * grid_height)
			y = y + 1;

		if (i > ver_cen[m + 1])
		{
			m = m + 1;
			if (m == grid_size_h)
				m = m - 1;
		}

		for (j = 0; j < width; j++)
		{
			if (j > (x + 1) * grid_width)
				x = x + 1;

			if (j > hor_cen[n + 1])
			{
				n = n + 1;
				if (n == grid_size_w)
					n = n - 1;
			}

			pos = i * width + j;
			curve_pos0 = m * grid_size_w + n;
			curve_pos1 = m * grid_size_w + n + 1;
			curve_pos2 = (m + 1) * grid_size_w + n;
			curve_pos3 = (m + 1) * grid_size_w + n + 1;

			//curve_pos0 = curve_pos1 = curve_pos2 = curve_pos3 = 8;

			temp_val = (int32_t)src[pos];

			// four corners

			if (i < ver_cen[0] && j < hor_cen[0]) // left top
			{
				idx = temp_val >> 8;

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];

				curve_delta = temp_val - (idx << 8);

				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = val0;

			}
			else if (i < ver_cen[0] && j > hor_cen[grid_size_w]) // right top
			{
				idx = temp_val >> 8;

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];

				curve_delta = temp_val - (idx << 8);

				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = val0;
			}
			else if (i > ver_cen[grid_size_h - 1] && j < hor_cen[0]) // left bottom
			{
				idx = temp_val >> 8;

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];

				curve_delta = temp_val - (idx << 8);

				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = val0;
			}
			else if (i > ver_cen[grid_size_h - 1] && j > hor_cen[grid_size_w - 1])	// right bottom
			{
				temp_val = src[pos];

				idx = temp_val >> 8;

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];

				curve_delta = temp_val - (idx << 8);

				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = val0;
			}

			// four sides
			else if (i < ver_cen[0] && j > hor_cen[0] && j < hor_cen[grid_size_w - 1])	// top
			{
				idx = temp_val >> 8;
				curve_delta = temp_val - (idx << 8);

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];
				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				curve_val0 = tone_curve[curve_pos1 * 129 + idx];
				curve_val1 = tone_curve[curve_pos1 * 129 + idx + 1];
				val1 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = linear_interp(hor_cen[n], hor_cen[n + 1], val0, val1, j);

			}
			else if (i > ver_cen[grid_size_h - 1] && j > hor_cen[0] && j < hor_cen[grid_size_w - 1])	// bottom
			{
				idx = temp_val >> 8;
				curve_delta = temp_val - (idx << 8);

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];
				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				curve_val0 = tone_curve[curve_pos1 * 129 + idx];
				curve_val1 = tone_curve[curve_pos1 * 129 + idx + 1];
				val1 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = linear_interp(hor_cen[n], hor_cen[n + 1], val0, val1, j);
			}
			else if (i > ver_cen[0] && i < ver_cen[grid_size_h - 1] && j < hor_cen[0]) // left
			{
				idx = temp_val >> 8;
				curve_delta = temp_val - (idx << 8);

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];
				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				curve_val0 = tone_curve[curve_pos2 * 129 + idx];
				curve_val1 = tone_curve[curve_pos2 * 129 + idx + 1];
				val1 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = linear_interp(ver_cen[m], ver_cen[m + 1], val0, val1, i);
			}
			else if (i > ver_cen[0] && i < ver_cen[grid_size_h - 1] && j > hor_cen[grid_size_w - 1]) // right
			{
				idx = temp_val >> 8;
				curve_delta = temp_val - (idx << 8);

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];
				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				curve_val0 = tone_curve[curve_pos2 * 129 + idx];
				curve_val1 = tone_curve[curve_pos2 * 129 + idx + 1];
				val1 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val = linear_interp(ver_cen[m], ver_cen[m + 1], val0, val1, i);
			}

			// center
			else
			{
				idx = temp_val >> 8;
				curve_delta = temp_val - (idx << 8);

				curve_val0 = tone_curve[curve_pos0 * 129 + idx];
				curve_val1 = tone_curve[curve_pos0 * 129 + idx + 1];
				val0 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				curve_val0 = tone_curve[curve_pos1 * 129 + idx];
				curve_val1 = tone_curve[curve_pos1 * 129 + idx + 1];
				val1 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val0 = linear_interp(hor_cen[n], hor_cen[n + 1], val0, val1, j);

				curve_val0 = tone_curve[curve_pos2 * 129 + idx];
				curve_val1 = tone_curve[curve_pos2 * 129 + idx + 1];
				val2 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				curve_val0 = tone_curve[curve_pos3 * 129 + idx];
				curve_val1 = tone_curve[curve_pos3 * 129 + idx + 1];
				val3 = ((curve_delta * (curve_val1 - curve_val0)) >> 8) + curve_val0;

				mapped_val1 = linear_interp(hor_cen[n], hor_cen[n + 1], val2, val3, j);

				mapped_val = linear_interp(ver_cen[m], ver_cen[m + 1], mapped_val0, mapped_val1, i);
			}

			mapped_val = mapped_val > 4095 ? 4095 : mapped_val;
			mapped_val = mapped_val < 0 ? 0 : mapped_val;

			dst[pos] = (uint16_t)mapped_val;
		}
	}
}

/*
*
* Local Tone Mapping
*
* input:15bit
* output:12bit
* histogram points:128
* tone curve points:129
*
*/
void ltm_process(uint16_t* src, int height, int width, uint16_t* dst)
{
	int i, j;

	const int grid_size_h = 3;
	const int grid_size_w = 3;

	int grid_height = height / grid_size_h;
	int grid_width = width / grid_size_w;

	int hor_sta[grid_size_w];
	int hor_end[grid_size_w];

	int ver_sta[grid_size_h];
	int ver_end[grid_size_h];

	int hor_cen[grid_size_w];
	int ver_cen[grid_size_h];

	int local_max_k = 6;	// u6.5 [0, 32]	0.1875
	int local_max_b = 100;
	int local_min_k = 3;	// u6.5 [0, 32]	0.09375
	int local_min_b = 15;

	// calc grid
	gen_grid_params(height, width, grid_size_h, grid_size_w, grid_height, grid_width, hor_sta, hor_end, ver_sta, ver_end, hor_cen, ver_cen);

	//std::cout << "hor start :" << hor_sta[0] << " " << hor_sta[1] << " " << hor_sta[2] << " " << hor_sta[3] << " " << hor_sta[4] << " " << hor_sta[5] << std::endl;
	//std::cout << "hor end   :" << hor_end[0] << " " << hor_end[1] << " " << hor_end[2] << " " << hor_end[3] << " " << hor_end[4] << " " << hor_end[5] << std::endl;
	//std::cout << "ver start :" << ver_sta[0] << " " << ver_sta[1] << " " << ver_sta[2] << " " << ver_sta[3] << " " << ver_sta[4] << " " << ver_sta[5] << std::endl;
	//std::cout << "ver end   :" << ver_end[0] << " " << ver_end[1] << " " << ver_end[2] << " " << ver_end[3] << " " << ver_end[4] << " " << ver_end[5] << std::endl;
	//std::cout << "hor center:" << hor_cen[0] << " " << hor_cen[1] << " " << hor_cen[2] << " " << hor_cen[3] << " " << hor_cen[4] << " " << hor_cen[5] << std::endl;
	//std::cout << "ver center:" << ver_cen[0] << " " << ver_cen[1] << " " << ver_cen[2] << " " << ver_cen[3] << " " << ver_cen[4] << " " << ver_cen[5] << std::endl;

	// spilit to base and detail layer
	// calc detail

	// histogram
	//int32_t* local_hist = new int32_t[grid_size_h * grid_size_w * 128];
	 int32_t local_hist[grid_size_h * grid_size_w * 128];
	memset(local_hist, 0, sizeof(int32_t) * grid_size_h * grid_size_w * 128);

	int32_t local_hist_limit[grid_size_h * grid_size_w * 128];
	memset(local_hist_limit, 0, sizeof(int32_t) * grid_size_h * grid_size_w * 128);

	for (i = 0; i < grid_size_h; i++)
	{
		for (j = 0; j < grid_size_w; j++)
		{
			gen_hist(src, height, width, ver_sta[i], ver_end[i], hor_sta[j], hor_end[j], local_hist + (i * grid_size_w + j) * 128);
			hist_max_and_min_limit(local_hist + (i * grid_size_w + j) * 128, 128, local_max_k, local_max_b, local_min_k, local_min_b, local_hist_limit + (i * grid_size_w + j) * 128);
		}
	}

	// histogram process
	//int32_t* local_cdf = new int32_t[grid_size_h * grid_size_w * 129];
	 int32_t local_cdf[grid_size_h * grid_size_w * 129];
	memset(local_cdf, 0, sizeof(int32_t) * grid_size_h * grid_size_w * 129);
	for (i = 0; i < grid_size_h; i++)
	{
		for (j = 0; j < grid_size_w; j++)
		{
			hist_cdf(local_hist_limit + (i * grid_size_w + j) * 128, 128, local_cdf + (i * grid_size_w + j) * 129);
		}
	}

	// bilinear interp
	bilinear_interp(src, local_cdf, height, width, grid_size_h, grid_size_w, grid_height, grid_width, hor_cen, ver_cen, dst);


	//delete[] local_hist;
	//delete[] local_cdf;
}


void run_ltm(uint16_t* src, int32_t height, int32_t width, uint16_t* dst)
{
	int i, j;
	bool bypass = false;

	if (bypass)
	{
		bypass_operator(src, height, width, dst, 4);
	}
	else
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				src[i * width + j] = src[i * width + j] >> 1;
			}
		}
		ltm_process(src, height, width, dst);
	}

}

