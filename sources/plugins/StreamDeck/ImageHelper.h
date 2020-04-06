#pragma once
#include <opencv2/opencv.hpp>

class CImageHelper
{
public:
	static std::vector<uchar> cvMatToVector(cv::Mat& image);
	static std::vector<uchar> stringToVector(std::string& content);
	static cv::Mat renderKeyImage(std::vector<uchar>& data, int keyPixelSize, std::string& customText);
	// Thanks to Lode Vandevenne https://lodev.org/lodepng/
	static void encodeBMP(std::vector<unsigned char>& bmp, const unsigned char* image, int w, int h);
};
