/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "visualize.h"
#include "io.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace cv;

namespace aire {

Visualize::Visualize(IO t_io) {
	io = t_io;
}

Visualize::Visualize(string str) {
	io = IO(str);
}


Visualize::~Visualize() {
	// TODO Auto-generated destructor stub
}

void Visualize::createCameraMotionImages(Video video){

	string imdir = "camera_motion_images";
	io.createDirectory(imdir);
	string image_dir = io.project_dir+"/"+imdir;

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
		string new_string = string(3 - itoa (i,buffer,10).length(), '0') + i;
		std::ostringstream ss;
		ss << std::setw(5) << std::setfill('0') << 12 << "\n";
		std::string s2(ss.str());
		char imf[200];
		sprintf(imf,"%s/%%03d.png",(const char*)image_dir);

		sprintf(imf,imf,i);
		cv::bitwise_and(src1,src2,output);
		cv::imwrite(imf,output);
	}
}
void Visualize::createCameraMotionGraphs(){
	vector<int> cc = io.readIntVector("camera_changes.txt");
	vector<Scalar> cm = io.readScalarVector("camera_motion.txt");
	Visualize::createCameraMotionGraphs(cc, cm);
}

void Visualize::createCameraMotionGraphs(vector<int> cc, vector<Scalar> cm){
	string imdir = "camera_motion_graphs";
	io.createDirectory(imdir);
	string image_dir = io.project_dir+"/"+imdir;
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

} /* namespace aire */
