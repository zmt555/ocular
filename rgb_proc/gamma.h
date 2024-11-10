#include<iostream>
#include<stdlib.h>

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif // !MAX

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif // !MIN

#ifndef CLIP
#define CLIP(x, a, b) (MAX((a), MIN((x), (b))))
#endif // !CLIP


void run_gamma(uint8_t* src, int32_t height, int32_t width, uint8_t* dst);
