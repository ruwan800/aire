#include "../mylibs/ImageInfoLoader.hpp"
#include "../mylibs/Util.h"

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;
using namespace cv;

/* Private methods */

Mat getBinaryImage(Mat bgrImage) {
	Mat hsvImage, binaryImage;
	cvtColor(bgrImage, hsvImage, CV_BGR2HSV);

	int iHighH = 44;
	int iLowH = 7;

	int iLowS = 15; //84
	int iHighS = 102; //97

	int iLowV = 115; //148
	int iHighV = 232; //166

	inRange(hsvImage, Scalar(iLowH, iLowS, iLowV),
			Scalar(iHighH, iHighS, iHighV), binaryImage);
	hsvImage.release();

//	imshow("img", binaryImage);
//
//	cvWaitKey(0);

	return binaryImage;
}

/* End of private methods */

/* public static methods */

double ImageInfoLoader::getEdgePercentage(Mat bgrFrame, int threshold, int maxValue, int width, int height){
	double percentage;
	double whitePixels = 0.0;
	Mat grayFrame;

	/* Converting BGR image to gray scale image */
	cvtColor(bgrFrame, grayFrame, CV_BGR2GRAY);

	double totalPixels = width * height;

	/* Applying Gaussian blur */
	GaussianBlur(grayFrame, grayFrame, Size(1,1), 2, 2);

	/* Extracting edges using Canny's algorithm */
	Canny(grayFrame, grayFrame, 50, 150, 3);

	/* Getting the amount of non-black pixels */
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(grayFrame.at<uchar>(i,j) >= 1){
				whitePixels += 1.0;
			}
		}
	}

	/* Calculating the percentage */
	percentage = (100.0 * whitePixels) / totalPixels;
//	imshow("aaaa", grayFrame);
//	cvWaitKey(0);

	//cout << percentage << endl;
	grayFrame.release();

	return percentage;
}

int ImageInfoLoader::isPitchAvailable(Mat bgrFrame, int width, int height,
		double* diameterOfPitch) {
	Mat binaryImage = getBinaryImage(bgrFrame);
	vector<KeyPoint> keyPoints;

	SimpleBlobDetector::Params params;
	params.minThreshold = 40; //40
	params.maxThreshold = 60; //60
	params.thresholdStep = 5;

	params.minArea = (width * height * 4) / 100;
	params.minConvexity = 0.3;
	params.minInertiaRatio = 0.01;

	params.maxArea = (width * height * 90) / 100;
	params.maxConvexity = 10;

	params.filterByColor = false;
	params.filterByCircularity = false;

	line(binaryImage, Point(0, binaryImage.rows - 1),
			Point(binaryImage.cols - 1, binaryImage.rows - 1), Scalar::all(255));

	SimpleBlobDetector blobDetector(params);
	blobDetector.create("SimpleBlob");

	blobDetector.detect(binaryImage, keyPoints);

	int numberOfBlobs = (int) keyPoints.size();

	if (numberOfBlobs > 0) {
		*diameterOfPitch = keyPoints.at(0).size;
	} else {
		*diameterOfPitch = 0;
	}

	binaryImage.release();
	return numberOfBlobs;
}

int ImageInfoLoader::getCameraFocusedArea(Mat bgrImge, int width, int height){
	const int GROUND = 0;
	const int SKY = 1;
	const int PAVILION = 2;

	int blue[255];
	int green[255];
	int red[255];

	int r, g, b;

	Util::makeZeroIntArray(blue, 255);
	Util::makeZeroIntArray(green, 255);
	Util::makeZeroIntArray(red, 255);

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			/* Getting BGR values of a particular pixel */
			int valueB = bgrImge.at<Vec3b>(j, i)[0];
			int valueG = bgrImge.at<Vec3b>(j, i)[1];
			int valueR = bgrImge.at<Vec3b>(j, i)[2];

			/* Incrementing the relevant color position */
			blue[valueB] ++;
			green[valueG] ++;
			red[valueR] ++;
		}
	}

	g = Util::getHigestIntPosition(green, 255);
	r = Util::getHigestIntPosition(red, 255);
	b = Util::getHigestIntPosition(blue, 255);

	double hue = Util::getHueFromBGR(b, g, r);
	double saturation = Util::getSaturationFromBGR(b, g, r);

	/* Deciding the focused place and labeling the frame */
	if((hue <= 162 && hue >= 60) && (saturation > 11)){
		//Ground
		return GROUND;
	} else if((hue <= 258 && hue > 162) || (saturation < 11)){
		// Sky
		return SKY;
	} else {
		//pavilion
		return PAVILION;
	}
}
