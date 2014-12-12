/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: aire
 */

#include "motion.h"
#include "io.h"
#include "log.h"
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;
using namespace cv;

namespace aire {

Motion::Motion(Video vid)
	:video(vid), LOG(video.LOG){}

Motion::~Motion() {
	// TODO Auto-generated destructor stub
}

std::vector<int> Motion::findCameraChanges() {

	std::vector<int> camPoints;
	camPoints.push_back(0);
	int last_value = 0;
	int short_transition_threshold = 30;
	int long_transition_threshold = 130;
	int possible_min_width = 10;

	Log::Process* pr = LOG->startProcess("Finding Camera Scenes");
	pr->setProcessBoundary(video.size()-1);

	for (int i = 0; i < (int)video.size()-8; ++i) {
		pr->setProcessBoundary(video.size()-1);
		pr->setProcessProgress(i+8);
		//std::cout << i << "/" << frames.size()-4 << std::endl;//####
		cv::Scalar m[4];
		std::vector<cv::Mat> frames = video.getFrames(i,i+8);
		for (int j = 0; j < 4; ++j) {
			cv::Mat d0,d1,r0,gray,thresh;
			if(j==0){
				cv::absdiff(frames.at(2), frames.at(3), d0);
				cv::absdiff(frames.at(2), frames.at(4), d1);
			}
			else if(j==1){
				cv::absdiff(frames.at(3), frames.at(5), d0);
				cv::absdiff(frames.at(4), frames.at(5), d1);
			}
			else if(j==2){
				cv::absdiff(frames.at(4), frames.at(5), d0);
				cv::absdiff(frames.at(4), frames.at(6), d1);
			}
			else if(j==3){
				cv::absdiff(frames.at(5), frames.at(7), d0);
				cv::absdiff(frames.at(6), frames.at(7), d1);
			}
			cv::bitwise_and(d0, d1, r0);
			cv::cvtColor(r0,gray,CV_BGR2GRAY);
			threshold(gray, thresh, 10, 255, CV_THRESH_BINARY);
			m[j] = cv::mean(thresh);
		}
		//std::cout << i+1 << "::++++::" << m[1][0]-m[0][0] << "::" << m[2][0]-m[3][0] << "::" << m[0] << m[1] << m[2] << m[3] << std::endl;//####
		if( short_transition_threshold < m[1][0] -m[0][0] && short_transition_threshold < m[2][0] - m[3][0]){
			if(possible_min_width < i+4-last_value || last_value == 0 ){
				camPoints.push_back(i+4);
				LOG->i("output",format("Camera Change: Frame: %4d",i+4));
				LOG->d("camera_change ",format("%2.3f, %2.3f, %2.3f, %2.3f",m[0][0], m[1][0], m[2][0], m[3][0]));
				//std::cout << i+1 << "::" << m[0] << m[1] << m[2] << std::endl;//####
			}
			last_value = i+4;
			continue;
		}
		for (int j = 0; j < 4; ++j) {
			cv::Mat d0,d1,r0,gray,thresh;
			if(j==0){
				cv::absdiff(frames.at(0), frames.at(2), d0);
				cv::absdiff(frames.at(1), frames.at(2), d1);
			}
			else if(j==1){
				cv::absdiff(frames.at(0), frames.at(5), d0);
				cv::absdiff(frames.at(1), frames.at(5), d1);
			}
			else if(j==2){
				cv::absdiff(frames.at(2), frames.at(7), d0);
				cv::absdiff(frames.at(2), frames.at(6), d1);
			}
			else if(j==3){
				cv::absdiff(frames.at(5), frames.at(7), d0);
				cv::absdiff(frames.at(5), frames.at(6), d1);
			}
			cv::bitwise_and(d0, d1, r0);
			cv::cvtColor(r0,gray,CV_BGR2GRAY);
			threshold(gray, thresh, 10, 255, CV_THRESH_BINARY);
			/*imshow("absdiff0",d0);//####
			imshow("absdiff1",d1);//####
			imshow("bitwise_and",thresh);//####
			waitKey(0);*/
			m[j] = cv::mean(thresh);
		}
		//std::cout << i+1 << "::----::" << m[1][0]-m[0][0] << "::" << m[2][0]-m[3][0] << "::" << m[0] << m[1] << m[2] << m[3] << std::endl;//####
		if( long_transition_threshold < m[1][0] -m[0][0] && long_transition_threshold < m[2][0] - m[3][0]){
			if(possible_min_width < i+4-last_value || last_value == 0 ){
				camPoints.push_back(i+4);
				LOG->i("output",format("Camera Change: Frame: %4d",i+4));
				LOG->d("camera_change (Long) ",format("%2.3f, %2.3f, %2.3f, %2.3f",m[0][0], m[1][0], m[2][0], m[3][0]));
				//std::cout << i+1 << "::" << m[0] << m[1] << m[2] << std::endl;//####
			}
			last_value = i+4;
		}
	}
	//if(possible_min_width < video.size() - last_value || last_value == 0){
	camPoints.push_back(video.size()-1);
	//}
	LOG->endProcess(*pr);
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

/*
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
*/

/*
void Motion::createCameraMotionGraphs(){
	IO io = IO(video);
	string imdir = "camera_motion_graphs";
	io.createDirectory(imdir);
	string image_dir = io.project_dir+"/"+imdir;
	vector<int> cc = io.readIntVector("camera_changes.txt");
	vector<Scalar> cm = io.readScalarVector("camera_motion.txt");
	int previous = 0;
	for ( int i = 0; i < (int) cc.size()+1; ++i) {
		Mat m(Size(800, 640), CV_8UC3);
		line(m, Point(10,10), Point(10,212), Scalar(255,255,255), 1);
		line(m, Point(10,220), Point(10,422), Scalar(255,255,255), 1);
		line(m, Point(10,430), Point(10,632), Scalar(255,255,255), 1);

		line(m, Point(10,110), Point(790,110), Scalar(255,255,255), 1);
		line(m, Point(10,320), Point(790,320), Scalar(255,255,255), 1);
		line(m, Point(10,530), Point(790,530), Scalar(255,255,255), 1);
		Scalar sc0 = cm.at(0);
		int currrent;
		if(i == cc.size()){
			currrent = cm.size();
		}
		else{
			currrent = cc.at(i);
		}
		for ( int j = 1; j < currrent -previous; ++j) {
			if(192 < currrent-previous)break;
			Scalar sc1 = cm.at(previous+j);
			for (int k = 0; k < 3; ++k) {
				line(m, Point(10+((j-1)*4), ((k*210)+110)-(2*sc0[k])), Point(10+(j*4), ((k*210)+110)-(2*sc1[k])), Scalar(0,0,200+abs(sc0[k])), 2);
			}
			sc0 = sc1;
		}
		char x[200];
		sprintf(x, (image_dir+"/%03d.png").c_str(),i);
		imwrite(x, m);
		if(i != cc.size()) previous = cc.at(i);
	}
}
*/


} /* namespace aire */
