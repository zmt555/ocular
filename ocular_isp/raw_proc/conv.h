#include<iostream>


void add_abs_mat(int16_t* mat1, int16_t* mat2, int32_t height, int32_t width, int16_t* dst);
void conv2d(int16_t* src, int32_t height, int32_t width, int* kernel, int kernel_size, int16_t* dst);
void conv2d(int16_t* src, int32_t height, int32_t width, int* kernel, int kernel_size, int shift, int16_t* dst);


