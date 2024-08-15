
#include<stdio.h>
#include<stdint.h>


void roberts_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void prewitt_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void sobel_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void scharr_operator(int16_t*src, int32_t height, int32_t width, int16_t* dst);

void laplace_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void gradient_operator(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void box_filter(int16_t* src, int32_t height, int32_t width, int16_t* dst);


