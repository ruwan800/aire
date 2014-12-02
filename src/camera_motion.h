/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "video.h"
#include "log.h"


#ifndef CAMERA_MOTION_H_
#define CAMERA_MOTION_H_

namespace aire {

class CameraMotion {
public:
	CameraMotion(Video, std::vector<int>);
	virtual ~CameraMotion();
	std::vector<cv::Scalar> findCameraMotion();
private:
	Video video;
	Log LOG;
	std::vector<int> cc;
	std::vector<cv::Scalar> cm;
	float mean_min;
	cv::Mat frame1;
	cv::Mat frame2;
	cv::Mat srcError;
	cv::Scalar meanMinVal;
	typedef std::map< int, cv::Mat > matMap;
	matMap matSet;
	matMap matSet1;
	cv::Scalar findCameraPoint(cv::Scalar initial_point);
	void countMean(cv::Scalar point);
};

} /* namespace aire */
#endif /* CAMERA_MOTION_H_ */
