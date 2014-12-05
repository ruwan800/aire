/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "log.h"


#ifndef CORE_H_
#define CORE_H_

namespace aire {

class Video {
public:
	//Video();
	//Video(bool);
	Video(string video_file, bool);
	virtual ~Video();
	int size();
	std::vector<cv::Mat> getFrames(int, int);
	cv::Mat getFrame(int);
	int getFrameRate();
	Log LOG;
	string video_file;
private:
	cv::VideoCapture capture;
	cv::Mat frame_set[2][100];
	int loaded_frames;
	int video_size;
	int frame_rate;
	bool video_load_complete;
	bool adjust_resolution;
	void loadVideo();
	void loadInitialFrames();
	void loadFrames(int);
	void load100Frames();
};

} /* namespace aire */
#endif /* CORE_H_ */
