#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	// Load the image in grayscale
	cv::Mat img = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
	if (img.empty()) {
		std::cerr << "Could not open the image!\n";
		return -1;
	}

	cv::Mat blur, edges;

	// Step 1: Apply Gaussian Blur to reduce noise
	cv::GaussianBlur(img, blur, cv::Size(5, 5), 1.4); // kernel size 5x5, sigma 1.4

	// Step 2: Apply Canny Edge Detection
	cv::Canny(blur, edges, 100, 200); // lower threshold: 100, upper: 200

	// Step 3: Show results
	std::string s1 = "Original (Grayscale)";
	std::string s2 = "Canny Edge Detection";
	cv::namedWindow(s1, cv::WINDOW_NORMAL);
	cv::namedWindow(s2, cv::WINDOW_NORMAL);
	cv::imshow(s1, img);
	cv::imshow(s2, edges);
	cv::waitKey(0); // Wait for a key press

	return 0;
}
