#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#ifndef IMAGEINFOLOADER_HPP_
#define IMAGEINFOLOADER_HPP_

using namespace cv;
class ImageInfoLoader{
public:
	static int isPitchAvailable(cv::Mat bgrImage, int width, int height, double* diameterOfPitch);
	static double getEdgePercentage(cv::Mat bgrImage, int threshold, int maxValue, int width, int height);
	static int getCameraFocusedArea(cv::Mat bgrImage, int width, int height);
};

#endif /* IMAGEINFOLOADER_HPP_ */
