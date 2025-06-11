// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "opencv2/opencv.hpp"
#include <fmt/core.h>

void filter(const cv::Mat& mat, cv::Mat& dst, const cv::Mat& mask);


int main()
{
	cv::Mat mat = cv::imread("Test.jpg", cv::IMREAD_GRAYSCALE);
	constexpr int nDimension = 3;
	cv::Mat blurred;
	cv::blur(mat, blurred, cv::Size(nDimension, nDimension));

	std::string blurredTitle = "Blurred";
	std::string matTitle = "Original";
	cv::namedWindow(blurredTitle, cv::WINDOW_NORMAL);
	cv::namedWindow(matTitle, cv::WINDOW_NORMAL);

	cv::imshow(matTitle, mat);
	cv::imshow(blurredTitle, blurred);
	cv::waitKey(0);
	
	return 0;
}

void filter(const cv::Mat& mat, cv::Mat& dst, const cv::Mat& mask)
{
	dst = cv::Mat(mat.size(), CV_32F, cv::Scalar(0));
	cv::Point h_m = mask.size() / 2;
	for (int filterYIdx = h_m.y; filterYIdx < mat.rows - h_m.y; filterYIdx++)
	{
		for (int filterXIdx = h_m.x; filterXIdx < mat.cols - h_m.x; filterXIdx++)
		{
			float fSum = 0.0f;
			for (int u = 0; u < mask.rows; u++)
			{
				for (int v = 0; v < mask.cols; v++)
				{
					const int targetY = filterYIdx + u - h_m.y;
					const int targetX = filterXIdx + v - h_m.x;
					if (targetY >= 0 && targetY < mat.rows && targetX >= 0 && targetX < mat.cols)
					{
						fSum += mat.at<uchar>(targetY, targetX) * mask.at<float>(u, v);
					}
				}
			}


			dst.at<float>(filterYIdx, filterXIdx) = fSum;
		}
	}
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
