/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: aire
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "video.h"
#include "camera_motion.h"
#include "log.h"


#ifndef MOTION_H_
#define MOTION_H_

namespace aire {

class Motion {
public:
	Motion(Video vid);
	virtual ~Motion();
	std::vector<int> findCameraChanges();
	std::vector<cv::Scalar> findCameraMotion();
	void createCameraMotionImages(const char*);
	void loadCameraMotionFromFile(const char*);
	void createCameraMotionGraphs();
	bool isNotSimilar(vector<cv::Mat> images);
private:
	Video video;
	Log* LOG;
	std::vector<int> camera_changes;
	std::vector<cv::Scalar> camera_movements;

};

} /* namespace aire */
#endif /* MOTION_H_ */
