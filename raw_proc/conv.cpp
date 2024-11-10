#include<iostream>

void add_abs_mat(int16_t* mat1, int16_t* mat2, int32_t height, int32_t width, int16_t* dst)
{
    int16_t val = 0;
    int i, j;
    int pos = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pos = i * width + j;
            val = abs(mat1[pos]) + abs(mat2[pos]);

            // val = (val > 16383) ? 16383 : val;

            dst[pos] = (int16_t)val;
        }
    }
}


int mirror_filter(int a, int b, int c)
{
    a = (a > b) ? a : (b - a);
    a = (a < c) ? a : (c - (a - c));

    return a;
}


void conv2d(int16_t* src, int32_t height, int32_t width, int* kernel, int kernel_size, int16_t* dst)
{

    int32_t val = 0;
    int pos = 0;

    int i, j, k;
    int ii, jj;
    int iii, jjj;

    int half_kernel_size = kernel_size >> 1;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {

            pos = i * width + j;
            val = 0;

            for (ii = 0; ii < kernel_size; ii++)
            {
                for (jj = 0; jj < kernel_size; jj++)
                {
                    iii = i + (ii - half_kernel_size);
                    jjj = j + (jj - half_kernel_size);

                    iii = mirror_filter(iii, 0, height - 1);
                    jjj = mirror_filter(jjj, 0, width - 1);

                    val += (int32_t)(src[iii * width + jjj]) * (int32_t)(kernel[ii * kernel_size + jj]);

                }
            }

            dst[pos] = (int16_t)val;
        }
    }
}

void conv2d(int16_t* src, int32_t height, int32_t width, int* kernel, int kernel_size, int shift, int16_t* dst)
{

    int32_t val = 0;
    int pos = 0;

    int i, j, k;
    int ii, jj;
    int iii, jjj;

    int half_kernel_size = kernel_size >> 1;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {

            pos = i * width + j;
            val = 0;

            for (ii = 0; ii < kernel_size; ii++)
            {
                for (jj = 0; jj < kernel_size; jj++)
                {
                    iii = i + (ii - half_kernel_size);
                    jjj = j + (jj - half_kernel_size);

                    iii = mirror_filter(iii, 0, height - 1);
                    jjj = mirror_filter(jjj, 0, width - 1);

                    val += (int32_t)(src[iii * width + jjj]) * (int32_t)(kernel[ii * kernel_size + jj]);

                }
            }

            val = val >> shift;
            dst[pos] = (int16_t)val;
        }
    }
}

