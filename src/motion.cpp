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

/*this is a test commit */

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
	int short_transition_threshold = 20;
	int long_transition_threshold = 130;
	int possible_min_width = 10;

	LOG->startSubProcess("Finding Camera Scenes");
	LOG->setSubProcessBoundary(video.size()-1);

	for (int i = 0; i < (int)video.size()-8; ++i) {
		LOG->setSubProcessBoundary(video.size()-1);
		LOG->setSubProcessProgress(i+8);
		//std::cout << i << "/" << frames.size()-4 << std::endl;//####
		cv::Scalar m[4];
		//Mat Temp01[4];//####
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
			//Temp01[j] = thresh;//####
		}
		/*imshow("frame0",Temp01[0]);//####
		imshow("frame1",Temp01[1]);//####
		imshow("frame2",Temp01[2]);//####
		imshow("frame3",Temp01[3]);//####
		waitKey();*/
		//std::cout << i+1 << "::++++::" << m[1][0]-m[0][0] << "::" << m[2][0]-m[3][0] << "::" << m[0] << m[1] << m[2] << m[3] << std::endl;//####
		if( short_transition_threshold < m[1][0] -m[0][0] && short_transition_threshold < m[2][0] - m[3][0]){
			if(possible_min_width < i+4-last_value || last_value == 0 ){

				//imshow("x2", frames.at(2));
				//imshow("x3", frames.at(3));
				//imshow("x4", frames.at(4));
				//imshow("x5", frames.at(5));
				//imshow("x6", frames.at(6));
				if(isNotSimilar(frames)){
					camPoints.push_back(i+4);
					LOG->i("output",format("Camera Change: Frame: %4d",i+4));
					LOG->d("camera_change ",format("%2.3f, %2.3f, %2.3f, %2.3f",m[0][0], m[1][0], m[2][0], m[3][0]));
					//std::cout << i+1 << "::" << m[0] << m[1] << m[2] << std::endl;//####
				}
				last_value = i+4;
				continue;
			}
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
	LOG->endSubProcess();
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


bool Motion::isNotSimilar(vector<Mat> images) {

	for (int i = 0; i < images.size()-1; ++i) {
		//int m_x = img_1.cols/2; //, m_y = img_1.rows/2;
		int minHessian = 10;
		Mat img_1 = images.at(i);
		Mat img_2 = images.at(i+1);

		SurfFeatureDetector detector( minHessian );

		std::vector<KeyPoint> keypoints_1, keypoints_2;

		detector.detect( img_1, keypoints_1 );
		detector.detect( img_2, keypoints_2 );

		//-- Step 2: Calculate descriptors (feature vectors)
		SurfDescriptorExtractor extractor;

		Mat descriptors_1, descriptors_2;

		extractor.compute( img_1, keypoints_1, descriptors_1 );
		extractor.compute( img_2, keypoints_2, descriptors_2 );

		//-- Step 3: Matching descriptor vectors using FLANN matcher
		FlannBasedMatcher matcher;
		std::vector< DMatch > matches;
		matcher.match( descriptors_1, descriptors_2, matches );

		//double max_dist = 0; double min_dist = 100;

		//-- Quick calculation of max and min distances between keypoints
		std::vector< DMatch > good_matches;
		vector<float> meanX;
		for( int i = 0; i < descriptors_1.rows; i++ ){
			//double dist = matches[i].distance;
			Point2f pt1 = keypoints_1.at(matches[i].queryIdx).pt;
			Point2f pt2 = keypoints_2.at(matches[i].trainIdx).pt;

			if(abs(pt1.x - pt2.x) < .5  && abs(pt1.y - pt2.y) < .5){
			//	circle(img_2, pt1, 2, Scalar(255,255,255), 1, 8, 0);
			//	line(img_2, pt1, pt2, Scalar(255,255,255), 1, 8, 0);
			}
			else if(abs(pt1.x - pt2.x) < 50  && abs(pt1.y - pt2.y) < 50 ){
				line(img_2, pt1, pt2, Scalar(0,255,0), 1, 8, 0);//#####
				circle(img_2, pt1, 2, Scalar(255,0,0), 1, 8, 0);//#####
				circle(img_2, pt2, 2, Scalar(0,0,255), 1, 8, 0);//#####
				good_matches.push_back( matches[i]);
				//meanX.push_back(pt1.x - pt2.x);
				//cout << matches[i].distance << endl;//####
				//cout << pt1.x - pt2.x << endl;//####
			}
			//cout << matches[i].distance << "::"<< matches[i].imgIdx << "::"<< "" << "::"<< matches[i].trainIdx << endl;
		}
		/*float meanX0 = 0;
		int start = meanX.size()/4;
		int finish = meanX.size()*3/4;
		for (int i = start; i < finish; ++i) {
			meanX0 += meanX.at(i);
		}
		float mX = meanX0*2/meanX.size();
		//cout << mX << endl;//####





		vector<float> meanZ;
		for( unsigned int i = 0; i < good_matches.size(); i++ ){

			Point2f pt_1 = keypoints_1.at(good_matches[i].queryIdx).pt;
			Point2f pt_2 = keypoints_2.at(good_matches[i].trainIdx).pt;
			float xd = pt_1.x - pt_2.x;
			float xz = (xd - mX)/(m_x - pt_1.x);
			meanZ.push_back(xz);
			//cout << xz << endl;
		}
		float meanZ0 = 0;
		start = meanZ.size()/4;
		finish = meanZ.size()*3/4;
		for (int i = start; i < finish; ++i) {
			meanZ0 += meanZ.at(i);
		}
		float mZ = meanZ0*2/meanZ.size();
		cout << mX << "-------" << mZ << endl;//####


		vector<Scalar> movements;*/
		//cout << "CP::" << Scalar(dX, dY, dZ ) << endl;

		//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
		//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
		//-- small)
		//-- PS.- radiusMatch can also be used here.
		//std::vector< DMatch > good_matches;

		//for( int i = 0; i < descriptors_1.rows; i++ ){
			//if( matches[i].distance <= max(2*min_dist, 0.02) ){

			//}
		//}

		//-- Draw only "good" matches
		//Mat img_matches;
		/*
		drawMatches( img_1, keypoints_1, img_2, keypoints_2,
				   matches, img_matches, Scalar::all(-1), Scalar::all(-1),
				   vector<char>(), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
		*/
		//-- Show detected matches
		//drawKeypoints(img_1,keypoints_1,img_matches,Scalar(255,0,0));
		//drawKeypoints(img_matches,keypoints_2,img_matches,Scalar(0,255,0));
		//imshow( "Good Matches", img_2 );

		//for( int i = 0; i < (int)good_matches.size(); i++ )	{
			//printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx );
		//}

		cout << "MATCHES..........................." << good_matches.size() << endl;//#####
		imshow("imfi0", img_1);//#####
		imshow("imfi3", img_2);//#####
		waitKey(0);


		//waitKey(0);
		if(good_matches.size() < 200){
			return true;
		}
	}
	return false;
}

} /* namespace aire */
