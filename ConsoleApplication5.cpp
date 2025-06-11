// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "opencv2/opencv.hpp"
#include <fmt/core.h>
#include <limits>

using CvImg = cv::Mat;

int main()
{
	CvImg img = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
	
	short data1[] =
		{
			0, 1, 0,
			1, -4, 1,
			0, 1, 0
		};

	short data2[] =
		{
			-1, -1, -1,
			-1, 8, -1,
			-1, -1, -1
		};

	CvImg dst1, dst2, dst3;
	CvImg laplacianMask4(3, 3, CV_16S, data1);
	CvImg laplacianMask8(3, 3, CV_16S, data2);

	cv::filter2D(img, dst1, CV_16S, laplacianMask4);
	cv::filter2D(img, dst2, CV_16S, laplacianMask8);
	cv::Laplacian(img, dst3, CV_16S, 1);

	cv::convertScaleAbs(dst1, dst1);
	cv::convertScaleAbs(dst2, dst2);
	cv::convertScaleAbs(dst3, dst3);

	std::string original = "original";
	std::string filer4D = "filer4D";
	std::string filer8D = "filer8D";
	std::string laplacian = "laplacianOpenCv";

	cv::namedWindow(original, cv::WINDOW_NORMAL);
	cv::namedWindow(filer4D, cv::WINDOW_NORMAL);
	cv::namedWindow(filer8D, cv::WINDOW_NORMAL);
	cv::namedWindow(laplacian, cv::WINDOW_NORMAL);

	cv::imshow(original, img);
	cv::imshow(filer4D, dst1);
	cv::imshow(filer8D, dst2);
	cv::imshow(laplacian, dst3);
	cv::waitKey(0);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu	
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
