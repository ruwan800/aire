/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include <iostream>
#include <stdio.h>
#include "camera_motion.h"

using namespace std;

namespace aire {

CameraMotion::CameraMotion(Video vid, std::vector<int> cc) {
	mean_min = 10000;
	video = vid;
	camera_changes = cc;
	Log LOG = video.LOG;
}

CameraMotion::~CameraMotion() {
	// TODO Auto-generated destructor stub
}

std::vector<cv::Scalar> CameraMotion::findCameraMotion() {

	cv::Scalar cp= cv::Scalar(0,0,0);
	std::vector<cv::Scalar> point_set;
	for (int i = 1; i < video.size(); ++i) {
		//std::cout << i << "/" << frames.size()-1 << std::endl;//####
		if(std::find(camera_changes.begin(), camera_changes.end(), i) == camera_changes.end()){
			std::vector<cv::Mat> frames = video.getFrames(i-1,i+1);
			frame1 = frames.at(0);
			frame2 = frames.at(1);
			cp = findCameraPoint(cp);
			point_set.push_back(cp);
			char output[200];
			sprintf(output,"X:%3d, Y:%3d, Z:%3d",(int)cp.val[0],(int)cp.val[1],(int)cp.val[2]);
			LOG.i("output",output);
		}else{
			cp = cv::Scalar(0,0,0);
			point_set.push_back(cp);
			LOG.i("output","X:  0, Y:  0, Z:  0");
		}
	}
	camera_movements = point_set;
	return point_set;
}


cv::Scalar CameraMotion::findCameraPoint(cv::Scalar initial_point){

	//std::cout << "@findCameraPoint" << std::endl;//####

	float lastMeanMin;
	mean_min = 10000;
	cv::Scalar lastMeanMinVal;
	meanMinVal = initial_point;

	int spts[18][3] =   {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1},
						 {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0},
						 {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
						 {0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
						 //{1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1},{-1,1,1},{-1,1,-1},{-1,-1,1},{-1,-1,-1}
						};

	cv::Mat de;
	cv::absdiff(frame1, frame2, de);
	cv::cvtColor( de, de, CV_BGR2GRAY );
	cv::threshold(de, srcError, 10, 255, CV_THRESH_BINARY);
	cv::blur(de,de,cv::Size(5,5),cv::Point(0,0),1);
	cv::threshold(de, srcError, 10, 255, CV_THRESH_BINARY);


	matSet = matSet1;
	matSet1.clear();
	do{
		lastMeanMinVal = meanMinVal;
		lastMeanMin = mean_min;
		if (mean_min == 10000){
			countMean(meanMinVal);
		}
		for (int i = 0; i < 6; ++i) {
			cv::Scalar cp(meanMinVal[0]+spts[i][0],meanMinVal[1]+spts[i][1],meanMinVal[2]+spts[i][2]);
			countMean(cp);
			if(mean_min < lastMeanMin && i%2 == 0){ continue; }
		}
		if(mean_min < lastMeanMin && lastMeanMinVal != meanMinVal){
			continue;
		}
		for (int i = 6; i < 10; ++i) {
			cv::Scalar cp(meanMinVal[0]+spts[i][0],meanMinVal[1]+spts[i][1],meanMinVal[2]+spts[i][2]);
			countMean(cp);
		}
		if(mean_min < lastMeanMin && lastMeanMinVal != meanMinVal){
			continue;
		}
		for (int i = 10; i < 14; ++i) {
			cv::Scalar cp(meanMinVal[0]+spts[i][0],meanMinVal[1]+spts[i][1],meanMinVal[2]+spts[i][2]);
			countMean(cp);
		}
		if(mean_min < lastMeanMin && lastMeanMinVal != meanMinVal){
			continue;
		}
		for (int i = 14; i < 18; ++i) {
			cv::Scalar cp(meanMinVal[0]+spts[i][0],meanMinVal[1]+spts[i][1],meanMinVal[2]+spts[i][2]);
			countMean(cp);
		}
	}while(mean_min < lastMeanMin && lastMeanMinVal != meanMinVal);

	////////////////////////////////////////////////////////////////////////////
	//temp++;
	//char text_file[200];
	//sprintf(text_file,imageset_directory,temp);
	//imwrite(text_file,test);
	////////////////////////////////////////////////////////////////////////////


	return meanMinVal;

}


void CameraMotion::countMean(cv::Scalar point){
	//std::cout << "@countMean" << std::endl;//####
	int Zcol = point[0];
	int Zrow = point[1];
	int Zzoom = point[2];
	if (50 < abs(Zcol) || 50 < abs(Zrow) || 20 < abs(Zzoom)){return;}
	int srcrows = frame1.rows, srccols = frame1.cols;
	cv::Mat src1 = frame1.clone(), src2 = frame2.clone(),srcE = srcError.clone();
	if(0 < Zzoom){
		if(matSet.find(Zzoom) != matSet.end()){
			src1 = matSet.find(Zzoom)->second;
		}
		else{
			cv::resize(src1,src1,cv::Size(srccols+(2*Zzoom),srcrows+(2*Zzoom)),0,0,cv::INTER_CUBIC);
			src1 = src1.colRange(Zzoom,src1.cols-Zzoom).rowRange(Zzoom,src1.rows-Zzoom);
			matSet[Zzoom] = src1;
		}
	}
	else if(Zzoom < 0){
		int Fzoom = abs(Zzoom);
		if(matSet1.find(Zzoom) != matSet1.end()){
			src2 = matSet1.find(Zzoom)->second;
		}
		else{
			cv::resize(src2,src2,cv::Size(srccols+(2*Fzoom),srcrows+(2*Fzoom)),0,0,cv::INTER_CUBIC);
			src2 = src2.colRange(Fzoom,src2.cols-Fzoom).rowRange(Fzoom,src2.rows-Fzoom);
			matSet1[Zzoom] = src2;
		}
		cv::resize(srcE,srcE,cv::Size(srcE.cols+(2*Fzoom),srcE.rows+(2*Fzoom)),0,0,cv::INTER_CUBIC);
		srcE = srcE.colRange(Fzoom,srcE.cols-Fzoom).rowRange(Fzoom,srcE.rows-Fzoom);
	}

	if(0 < Zcol){
		src1 = src1.colRange(Zcol, srccols);
		src2 = src2.colRange(0, srccols -Zcol);
		srcE = srcE.colRange(0, srccols -Zcol);
	}
	else if (Zcol < 0){
		int Fcol = abs(Zcol);
		src1 = src1.colRange(0, srccols -Fcol);
		src2 = src2.colRange(Fcol, srccols);
		srcE = srcE.colRange(Fcol, srccols);
	}

	if(0 < Zrow){
		src1 = src1.rowRange(0, srcrows -Zrow);
		src2 = src2.rowRange(Zrow, srcrows);
		srcE = srcE.rowRange(Zrow, srcE.rows);
	}
	else if(Zrow < 0){
		int Frow = abs(Zrow);
		src1 = src1.rowRange(Frow, srcrows);
		src2 = src2.rowRange(0, srcrows -Frow);
		srcE = srcE.rowRange(0, srcE.rows -Frow);
	}

	cv::Mat de, od;
	cv::absdiff(src1, src2, od);

	cv::cvtColor( od, od, CV_BGR2GRAY );
	cv::threshold(od, od, 35, 255, CV_THRESH_BINARY);
	cv::bitwise_and(od,srcE,od);

	float mean_out = cv::mean(od)[0]*((srccols*srcrows)/((srccols-abs(Zcol))*(srcrows-abs(Zrow))));
	if(mean_out < mean_min){
		mean_min = mean_out;
		meanMinVal = cv::Scalar(Zcol,Zrow,Zzoom);
	}
}

} /* namespace aire */
