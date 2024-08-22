#include<opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>

#include"raw_proc/raw_process.h"

using namespace std;
using namespace cv;

void main()
{

	int height = 1080;
	int width = 1920;

	string param_path = "G:/ISP/ocular/test_image/RAW/Sony_A74/DSC03767.json";


	FILE* fp = NULL;
	fp = fopen("G:/ISP/ocular/test_image/RAW/Sony_A74/DSC03767_1920x1080.raw", "rb+");
	string save_path = "G:/ISP/ocular/dst/DSC03767_ocular_out.jpg";

	uint16_t* data = new uint16_t[height * width];
	memset(data, 0, width * height * sizeof(uint16_t));
	fread(data, sizeof(uint16_t), width * height, fp);

	//int16_t* test_res = new int16_t[height * width];
	//memset(test_res, 0, width * height * sizeof(int16_t));
	//test_function(data, height, width, test_res);

	uint8_t* raw_res = new uint8_t[3 * height * width];
	memset(raw_res, 0, 3 * width * height * sizeof(uint8_t));

	raw_proc(data, param_path, height, width, raw_res);

	Mat img;
	Mat raw_domain;
	//Mat test_out;
	int bufLen = width * height;
	
	img.create(height, width, CV_16UC1);
	raw_domain.create(height, width, CV_8UC3);
	//test_out.create(height, width, CV_16SC1);

	memcpy(img.data, data, bufLen * sizeof(uint16_t));
	memcpy(raw_domain.data, raw_res, 3 * bufLen * sizeof(uint8_t));
	//memcpy(test_out.data, test_res, bufLen * sizeof(int16_t));

	imwrite(save_path, raw_domain);

	//namedWindow("img", 0);
	//imshow("img", img);

	namedWindow("raw_out", WINDOW_FREERATIO);
	// imshow("test_out", test_out);
	// imshow("raw_out", img);
	imshow("raw_out", raw_domain);

	waitKey(0);

}

