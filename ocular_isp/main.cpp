#include<opencv.hpp>

using namespace std;
using namespace cv;

void main()
{
	Mat image = imread("../ocular/test_image/RGB/Sony_A74/DSC05662.JPG");

	if (image.empty())
	{
		cout << "not" << endl;
	}

	namedWindow("test", 1);
	imshow("test", image);
	waitKey(0);

}

