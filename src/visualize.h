/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "video.h"
#include "camera_motion.h"
#include "log.h"
#include "io.h"


#ifndef VISUALIZE_H_
#define VISUALIZE_H_

namespace aire {

class Visualize {
public:
	Visualize();
	Visualize(IO);
	Visualize(string str);
	virtual ~Visualize();
	//std::vector<int> findCameraChanges();
	//std::vector<cv::Scalar> findCameraMotion();
	void createCameraMotionImages(Video video);
	void createCameraMotionGraphs();
	void createCameraMotionGraphs2();
private:
	void loadCameraMotionFromFile();
	void loadCameraChangeFromFile();
	IO io;
	std::vector<int> camera_changes;
	std::vector<cv::Scalar> camera_movements;

};

} /* namespace aire */
#endif /* VISUALIZE_H_ */
