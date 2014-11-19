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
#include "camera_motion.h"
#include "log.h"
#include "io.h"


#ifndef MOTION_H_
#define MOTION_H_

namespace aire {

class Visualize {
public:
	Visualize();
	Visualize(IO);
	Visualize(string str);
	virtual ~Visualize();
	std::vector<int> findCameraChanges();
	std::vector<cv::Scalar> findCameraMotion();
	void createCameraMotionImages(Video video);
	void loadCameraMotionFromFile(const char*);
	void createCameraMotionGraphs(vector<int> cc, vector<cv::Scalar> cm);
	void createCameraMotionGraphs();
private:
	IO io;
	std::vector<int> camera_changes;
	std::vector<cv::Scalar> camera_movements;

};

} /* namespace aire */
#endif /* MOTION_H_ */