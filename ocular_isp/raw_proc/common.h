#pragma once

#include<iostream>



typedef enum tag_bayer_pattern
{
	RGGB = 0,
	GRBG,
	GBRG,
	BGGR,
} BAYER_PATTERN;


typedef enum tag_raw_pixel_type
{
	R = 0,
	Gr,
	Gb,
	B,
	RAW_PIXEL_TYPE_BUTT,
} RAW_PIXEL_TYPE;



RAW_PIXEL_TYPE get_raw_pixel_type(const BAYER_PATTERN pattern, const int s32x, const int s32y);


void bayer_to_gray(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void bypass_operator(uint16_t* src, int32_t height, int32_t width, uint16_t* dst);




