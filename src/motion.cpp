/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "motion.h"
#include <iostream>
#include <stdio.h>

using namespace std;

namespace aire {

Motion::Motion(Video vid) {
	video = vid;
	LOG = vid.LOG;
}

Motion::~Motion() {
	// TODO Auto-generated destructor stub
}

std::vector<int> Motion::findCameraChanges() {

	std::vector<int> camPoints;


	for (int i = 2; i < (int)video.size()-2; ++i) {
		//std::cout << i << "/" << frames.size()-4 << std::endl;//####
		cv::Scalar m[3];
		std::vector<cv::Mat> frames = video.getFrames(i-2,i+3);
		for (int j = 0; j < 3; ++j) {
			cv::Mat d0,d1,r0,gray,thresh;
			cv::absdiff(frames.at(j), frames.at(j+2), d0);
			cv::absdiff(frames.at(j+1), frames.at(j+2), d1);
			cv::bitwise_and(d0, d1, r0);
			cv::cvtColor(r0,gray,CV_BGR2GRAY);
			threshold(gray, thresh, 10, 255, CV_THRESH_BINARY);
			m[j] = cv::mean(thresh);
		}
		if( 20 < m[1][0] -m[0][0] && 20 < m[1][0] - m[2][0]){
			camPoints.push_back(i+1);
			char output[200];
			sprintf(output,"Camera Change: Frame:%2d",i+1);
			LOG.i("output",output);
			char d_output[200];
			sprintf(d_output,"%2.3f, %2.3f, %2.3f, ",m[0][0], m[1][0], m[2][0]);
			LOG.d("camera_change",d_output);
			//std::cout << i+1 << "::" << m[0] << m[1] << m[2] << std::endl;//####
		}
	}
	camera_changes = camPoints;
	return camPoints;
}

std::vector<cv::Scalar> Motion::findCameraMotion(){
	if(! camera_changes.size()){
		findCameraChanges();
	}
	CameraMotion cm = CameraMotion(video, camera_changes);
	camera_movements = cm.findCameraMotion();
	return camera_movements;
}


void Motion::createCameraMotionImages(const char* src_dir){

	if(! camera_changes.size()){
		findCameraMotion();
	}
	for (int i = 1; i < (int)camera_movements.size(); ++i) {
		//std::cout << i << "/" << camera_movements.size()-1 << std::endl;//####
		cv::Scalar point = camera_movements.at(i);
		int Zcol = point[0];
		int Zrow = point[1];
		int Zzoom = point[2];
		cv::Mat src1 = video.getFrame(i-1).clone(), src2 = video.getFrame(i).clone();
		int srcrows = src1.rows, srccols = src1.cols;
		if(0 < Zzoom){
			cv::resize(src1,src1,cv::Size(srccols+(2*Zzoom),srcrows+(2*Zzoom)),0,0,cv::INTER_CUBIC);
			src1 = src1.colRange(Zzoom,src1.cols-Zzoom).rowRange(Zzoom,src1.rows-Zzoom);
		}
		else if(Zzoom < 0){
			int Fzoom = abs(Zzoom);
			cv::resize(src2,src2,cv::Size(srccols+(2*Fzoom),srcrows+(2*Fzoom)),0,0,cv::INTER_CUBIC);
			src2 = src2.colRange(Fzoom,src2.cols-Fzoom).rowRange(Fzoom,src2.rows-Fzoom);
		}

		if(0 < Zcol){
			src1 = src1.colRange(Zcol, srccols);
			src2 = src2.colRange(0, srccols -Zcol);
		}
		else if (Zcol < 0){
			int Fcol = abs(Zcol);
			src1 = src1.colRange(0, srccols -Fcol);
			src2 = src2.colRange(Fcol, srccols);
		}

		if(0 < Zrow){
			src1 = src1.rowRange(0, srcrows -Zrow);
			src2 = src2.rowRange(Zrow, srcrows);
		}
		else if(Zrow < 0){
			int Frow = abs(Zrow);
			src1 = src1.rowRange(Frow, srcrows);
			src2 = src2.rowRange(0, srcrows -Frow);
		}

		cv::Mat output;
		char imf[200];
		sprintf(imf,"%s/%%03d.png",src_dir);
		sprintf(imf,imf,i);
		cv::bitwise_and(src1,src2,output);
		cv::imwrite(imf,output);
	}
}

void loadCameraMotionFromFile(const char* src_file){

}

} /* namespace aire */
