#include<opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>

#include"raw_proc/raw_process.h"

using namespace std;
using namespace cv;

void main()
{

	int height = 1080;
	int width = 1920;

	FILE* fp = NULL;
	fp = fopen("G:/ISP/ocular/test_image/RAW/Sony_A74/DSC03767_1920x1080.raw", "rb+");
	int16_t* data = new int16_t[height * width];
	memset(data, 0, width * height * sizeof(int16_t));
	fread(data, sizeof(int16_t), width * height, fp);

	int16_t* test_res = new int16_t[height * width];
	memset(test_res, 0, width * height * sizeof(int16_t));

	test_function(data, height, width, test_res);

	Mat img;
	Mat test_out;
	int bufLen = width * height;
	
	img.create(height, width, CV_16SC1);
	test_out.create(height, width, CV_16SC1);

	memcpy(img.data, data, bufLen * sizeof(int16_t));
	memcpy(test_out.data, test_res, bufLen * sizeof(int16_t));

	//namedWindow("img", 0);
	//imshow("img", img);

	namedWindow("test_out", 0);
	imshow("test_out", test_out);

	waitKey(0);

}

