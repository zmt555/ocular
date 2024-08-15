/*
* This file contains multiple spatial filters, such as
* 
* sobel operator
* laplace operator
* median filter
* box filter
* guide filter
* bilateral filter
* directional bilateral filter
* gaussian filter
* non-local mean filter
* 
*/

#include<stdio.h>
#include<stdint.h>
#include<iostream>
#include<opencv.hpp>
#include"conv.h"
#include"common.h"



void roberts_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{

    int16_t* HorEdge = new int16_t[height * width];
    int16_t* VerEdge = new int16_t[height * width];

    int Gx[4] = {
        -1, 0,
        0, 1
    };

    int Gy[4] = {
        0, -1,
        1, 0
    };

    conv2d(src, height, width, Gx, 2, HorEdge);
    conv2d(src, height, width, Gy, 2, VerEdge);

    add_abs_mat(HorEdge, VerEdge, height, width, dst);

    delete [] HorEdge;
    delete [] VerEdge;

}


void prewitt_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{
    int16_t* HorEdge = new int16_t[height * width];
    int16_t* VerEdge = new int16_t[height * width];

    memset(HorEdge, 0, height * width * sizeof(int16_t));
    memset(VerEdge, 0, height * width * sizeof(int16_t));

    int Gx[9] = {
        -1, 0, 1,
        -1, 0, 1,
        -1, 0, 1
    };

    int Gy[9] = {
        -1, -1, -1,
        0, 0, 0,
        1, 1, 1
    };

    conv2d(src, height, width, Gx, 3, HorEdge);
    conv2d(src, height, width, Gy, 3, VerEdge);

    add_abs_mat(HorEdge, VerEdge, height, width, dst);

    //int target_height = 500;
    //int target_width = 100;

    //printf("target coordinate information:\n");
    //printf("original information\n");
    //for (int i = -1; i < 2; i++)
    //{
    //    for (int j = -1; j < 2; j++)
    //    {
    //        printf("%d ", src[(target_height + i) * width + target_width + j]);
    //    }
    //    printf("\n");
    //}

    //printf("horizontal:\n");
    //printf("%d\n", HorEdge[target_height * width + target_width]);

    //printf("vertical:\n");
    //printf("%d\n", VerEdge[target_height * width + target_width]);

    //printf("edge:\n");
    //printf("%d\n", dst[target_height * width + target_width]);


    delete [] HorEdge;
    delete [] VerEdge;
}


void sobel_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{
    int Gy[9] = {
        1, 2, 1,
        0, 0, 0,
        -1, -2, -1
    };
    
    int Gx[9] = {
        1, 0, -1,
        2, 0, -2,
        1, 0, -1
    };

    int16_t* HorEdge = new int16_t[height * width];
    int16_t* VerEdge = new int16_t[height * width];

    conv2d(src, height, width, Gx, 3, HorEdge);
    conv2d(src, height, width, Gy, 3, VerEdge);

    add_abs_mat(HorEdge, VerEdge, height, width, dst);


    delete [] HorEdge;
    delete [] VerEdge;

}


void scharr_operator(int16_t*src, int32_t height, int32_t width, int16_t* dst)
{
    int Gx[9] = {
        -3, 0, 3,
        -10, 0, 10,
        -3, 0, 3
    };

    int Gy[9] = {
        3, 10, 3,
        0, 0, 0,
        -3, -10, -3
    };

    int16_t* HorEdge = new int16_t[height * width];
    int16_t* VerEdge = new int16_t[height * width];

    conv2d(src, height, width, Gx, 3, HorEdge);
    conv2d(src, height, width, Gy, 3, VerEdge);

    add_abs_mat(HorEdge, VerEdge, height, width, dst);


    delete [] HorEdge;
    delete [] VerEdge;

}


void laplace_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{
    int kernel_4[9] = {
        0, -1, 0,
        -1, 4, -1,
        0, -1, 0
    };

    int kernel_8[9] = {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    };

    // conv2d(src, height, width, kernel_4, 3, dst);
    conv2d(src, height, width, kernel_8, 3, dst);

}


void gradient_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{

}


void canny_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{

}


void box_filter(int16_t* src, int32_t height, int32_t width, int16_t* dst)
{
    //int kernel_3[9] = {
    //    1, 1, 1,
    //    1, 1, 1,
    //    1, 1, 1
    //};

    //int kernel_5[25] = {
    //    1, 1, 1, 1, 1,
    //    1, 1, 1, 1, 1,
    //    1, 1, 1, 1, 1,
    //    1, 1, 1, 1, 1,
    //    1, 1, 1, 1, 1
    //};

    int kernel_2[4] = {
        1, 1,
        1, 1
    };

    int kernel_4[16] = {
        1, 1, 1, 1, 
        1, 1, 1, 1, 
        1, 1, 1, 1, 
        1, 1, 1, 1
    };

    // conv2d(src, height, width, kernel_2, 2, 2, dst);
    conv2d(src, height, width, kernel_4, 4, 4, dst);

}





