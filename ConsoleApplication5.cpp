// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "opencv2/opencv.hpp"
#include <fmt/core.h>
#include <limits>

using CvImg = cv::Mat;

void filter(const cv::Mat& mat, cv::Mat& dst, const cv::Mat& mask);

void homogenOp(cv::Mat targetImg, cv::Mat& dst, int maskSize);

void differOp(cv::Mat targetImg, cv::Mat& dst, int maskSize);

void differential(const CvImg& target, CvImg& dst, float data1[], float data2[]);

int main()
{
	cv::Mat mat = cv::imread("Test.jpg", cv::IMREAD_GRAYSCALE);
	float prewitVertical[] =
	{
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f
	};

	float prewitHorizontal[] =
	{
		-1.0f, -1.0f, -1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f
	};

	float sobelVertical[] =
	{
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f
	};

	float sobelHorizontal[] =
	{
		-2.0f, -2.0f, -2.0f,
		0.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 2.0f
	};

	float laplacianMaskFour[] =
	{
		0.0f, -1.0f, 0.0f,
		-1.0f, 4.0f, -1.0f,
		0.0f, -1.0f, 0.0f
	};

	float laplacianMaskEight[] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f, 8.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	};

	std::string original = "Original";
	std::string prewit = "Prewit Mask";
	std::string sobel = "Sobel Mask";
	std::string laplacianFour = "Laplacian Mask 4";
	std::string laplacianEight = "Laplacian Mask 8";
	cv::namedWindow(original, cv::WINDOW_NORMAL);
	cv::namedWindow(prewit, cv::WINDOW_NORMAL);
	cv::namedWindow(sobel, cv::WINDOW_NORMAL);
	cv::namedWindow(laplacianFour, cv::WINDOW_NORMAL);
	cv::namedWindow(laplacianEight, cv::WINDOW_NORMAL);
	
	CvImg laplacianFourDst;
	differential(mat, laplacianFourDst, laplacianMaskFour, laplacianMaskFour);

	CvImg laplacianEightDst;
	differential(mat, laplacianEightDst, laplacianMaskEight, laplacianMaskEight);
	
	cv::imshow(original, mat);
	cv::imshow(laplacianFour, laplacianFourDst);
	cv::imshow(laplacianEight, laplacianEightDst);
	
	cv::waitKey();
	cv::destroyAllWindows();
	
	return 0;
}

void filter(const cv::Mat& mat, cv::Mat& dst, const cv::Mat& mask)
{
	dst = cv::Mat(mat.size(), CV_8U, cv::Scalar(0));
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

			dst.at<uchar>(filterYIdx, filterXIdx) = static_cast<uchar>(fSum);
		}
	}
}

void homogenOp(cv::Mat targetImg, cv::Mat& dst, int maskSize)
{
	dst = cv::Mat(targetImg.size(), CV_8U, cv::Scalar(0));
	cv::Point maskingCenter(maskSize / 2, maskSize / 2);
	for (int i = maskingCenter.y; i < targetImg.rows - maskingCenter.y; i++)
	{
		for (int j = maskingCenter.x; j < targetImg.cols - maskingCenter.x; j++)
		{
			uchar max = std::numeric_limits<uchar>::min();
			for (int u = 0; u < maskSize; u++)
			{
				for (int v = 0; v < maskSize; v++)
				{
					const int targetY = i + u - maskingCenter.y;
					const int targetX = j + v - maskingCenter.x;
					uchar difference = abs(targetImg.at<uchar>(i, j) - targetImg.at<uchar>(targetY, targetX));
					if (difference > max)
						max = difference;
				}
			}
		}
	}
}

void differOp(cv::Mat targetImg, cv::Mat& dst, int maskSize)
{
	dst = cv::Mat(targetImg.size(), CV_8U, cv::Scalar(0));
	cv::Point maskingCenter(maskSize / 2, maskSize / 2);
	const int maskLenght = maskSize * maskSize;
	for (int i = maskingCenter.y; i < targetImg.rows - maskingCenter.y; i++)
	{
		for (int j = maskingCenter.x; j < targetImg.cols - maskingCenter.x; j++)
		{
			std::vector<uchar> mask{};
			mask.reserve(maskLenght);
			for (int u = 0, k = 0; u < maskSize; u++)
			{
				for (int v = 0; v < maskSize; v++, k++)
				{
					const int targetY = i + u - maskingCenter.y;
					const int targetX = j + v - maskingCenter.x;
					uchar pixelValue = targetImg.at<uchar>(targetY, targetX);
					mask.push_back(pixelValue);
				}
			}

			uchar max = std::numeric_limits<uchar>::min();
			for (int k = 0; k < maskLenght / 2; k++)
			{
				const int start = mask[k];
				const int end = mask[maskLenght - k - 1];
				const uchar difference = abs(start - end);
				max = std::max(difference, max);
			}

			dst.at<uchar>(i, j) = max;
		}
	}
}

void differential(const CvImg& target, CvImg& dst, float data1[], float data2[])
{
	CvImg dst1;
	CvImg dst2;

	CvImg mask1(3, 3, CV_32F, data1);
	CvImg mask2(3, 3, CV_32F, data2);

	cv::filter2D(target, dst1, CV_32F, mask1);
	cv::filter2D(target, dst2, CV_32F, mask2);
	cv::magnitude(dst1, dst2, dst);
	dst.convertTo(dst, CV_8U);

	cv::convertScaleAbs(dst1, dst1);
	cv::convertScaleAbs(dst2, dst2);
	std::string horizontalMask = "Horizontal Mask";
	std::string verticalMask = "Vertical Mask";
	cv::namedWindow(horizontalMask, cv::WINDOW_NORMAL);
	cv::namedWindow(verticalMask, cv::WINDOW_NORMAL);
	cv::imshow(horizontalMask, dst1);
	cv::imshow(verticalMask, dst2);
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
