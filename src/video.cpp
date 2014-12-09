/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "video.h"
#include <iostream>

using namespace std;
using namespace cv;

namespace aire {

//Video::Video(){
//}

/*Video::Video(string vf, bool adjust) {
	adjust_resolution = adjust;
	video_file = vf;
	video_size = 0;
	loadVideo();
	loadInitialFrames();
	Log log = Log();
	LOG = &log;
}*/

Video::Video(Log* log, string vf, bool adjust)
	:LOG(log)
{
	adjust_resolution = adjust;
	video_file = vf;
	video_size = 0;
	loadVideo();
	loadInitialFrames();
	capture_released = false;
}


/*Video::Video(bool adjust){
	adjust_resolution = adjust;
	video_size = 0;
	video_load_complete=false;
	loaded_frames = 0;
	video_file = "";
	LOG = Log();
}*/





Video::~Video(){}


int Video::size(){
	return video_size;
}

void Video::loadVideo(){
	LOG->i("Video",string("open"));
	video_load_complete = false;
	capture_released = false;
	loaded_frames = 0;
	capture = VideoCapture(video_file.c_str());	// open video file
	if(!capture.isOpened()){  			// check if we succeeded
		cout << "Video::Video::failed to open '" << video_file << "'." << endl;
		return;
	}
	//video_size = capture.get(CV_CAP_PROP_FRAME_COUNT);
	frame_rate = capture.get(CV_CAP_PROP_FPS);
}

int Video::getFrameRate(){
	return frame_rate;
}

std::vector<cv::Mat> Video::getFrames(int begin, int end){
	//cout << "@Video::getFrames:"<< begin << "," << end << endl;  //#####
	std::vector<cv::Mat> result;
	int complete = 0;
	if(60 < end - begin){
		cout << "Video::getFrames::too much frames requested at once." << endl;
	}
	if(begin < 0 || size() < end){
		cout << "Video::getFrames::requested out of existing frames range." << endl;
	}
	if(size() <= end+40){
		loadFrames(end);
	}
	if(loaded_frames <= end){
		loadFrames(end);
	}
	if(begin < loaded_frames-200){
		loadFrames(begin);
	}
	for (int i = 0; i < 2; ++i) {
		if(begin-end == complete) {break; }
		for (int j = complete; j < end-begin; ++j) {
			if(loaded_frames+(i*100)-200 <= begin+j && begin+j < loaded_frames+(i*100)-100){
				result.push_back(frame_set[i][begin+j-loaded_frames-(i*100)+200]);
				if(begin-end == complete) {break; }
				complete++;
			}
		}
	}
	return result;
}


cv::Mat Video::getFrame(int position){
	//cout << "@Video::getFrame:" << position << "::" << size() << "::" << loaded_frames << endl;  //#####
	if(position < 0 || size() < position){
		cout << "Video::getFrames::requested out of existing frame range." << endl;
	}
	if(size() <= position+40 && !video_load_complete){
		loadFrames(position);
	}
	if(loaded_frames <= position){
		loadFrames(position);
	}
	if(position < loaded_frames-200){
		loadFrames(position);
	}
	if(position < loaded_frames-100){
		return frame_set[0][position-loaded_frames+200];
	}
	if(position < loaded_frames){
		return frame_set[1][position-loaded_frames+100];
	}
	return Mat();
}


void Video::loadInitialFrames(){
	//cout << "@Video::loadInitialFrames" << endl;  //#####
	load100Frames();
	if(video_load_complete){return;}
	for (int j = 0; j < 100; ++j) {		//TODO make this efficient
		frame_set[0][j] = frame_set[1][j];
	}
	load100Frames();
}


void Video::load100Frames(){

	//cout << "@Video::load100Frames ::" << loaded_frames+100 << endl;  //#####
	if(video_load_complete){return;}
	for (int i = 0; i < 100; ++i) {
		cv::Mat frame;
		capture >> frame;				// get a new frame from camera
		if(frame.empty()){
			video_load_complete = true;
			break;
		}
		if(adjust_resolution){
			Mat new_frame;
			resize(frame,new_frame,Size(frame.cols*400/frame.rows,400),0,0,INTER_CUBIC);
			frame_set[1][i] = new_frame;
		}
		else{
			frame_set[1][i] = frame.clone();
		}
		if(video_size-1 < loaded_frames+i){
			video_size = loaded_frames+i+1;
		}
	}
	loaded_frames += 100;
}


void Video::loadFrames(int start){
	//cout << "@Video::loadFrames: "<< start << endl;  //#####
	//while(start+100 < loaded_frames-200 || loaded_frames < start+100){
	while(start+100 <= loaded_frames-200 || loaded_frames < start+100){
		if(loaded_frames < start+100){
			if(video_load_complete){
				release();
				break;
			}
			for (int i = 0; i < 100; ++i) {		//TODO make this efficient
				frame_set[0][i] = frame_set[1][i];
			}
			load100Frames();
		}
		//else if(start < loaded_frames-200){
		else if(start <= loaded_frames-200){
			loadVideo();
			loadInitialFrames();
		}
	}
}

bool Video::release(){
	if(capture.isOpened()){
		capture.release();
		capture_released = true;
		LOG->i("Video",string("release"));
		return true;
	}
	else{
		capture_released = true;
		return false;
	}
}

} /* namespace aire */
