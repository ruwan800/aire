#include "mylibs/Util.h"
#include "mylibs/ImageInfoLoader.hpp"
#include "mylibs/DataPreProcessor.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <aire/edge.h>
#include <aire/video.h>
#include <aire/motion.h>
#include <dirent.h>

using namespace cv;
using namespace std;
using namespace aire;

Mat getFrame(VideoCapture capture, int skipFrames){
	Mat frame;

	for(int i = 0; i < (skipFrames + 1); i++){
		capture >> frame;
	}

	return frame;
}

void cleanArrays(int* pitchAvailability, double* pitchSizes, double* edgePercentages, int* cameraFocusedArea, int size){
	Util::makeZeroIntArray(pitchAvailability, size);
	Util::makeZeroIntArray(cameraFocusedArea, size);
	Util::makeZeroDoubleArray(pitchSizes, size);
	Util::makeZeroDoubleArray(edgePercentages, size);
}

void getEdgePres(string newLoc){

	vector<vector <float> > data;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (newLoc.c_str())) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
	    //printf ("%s\n", ent->d_name);
		  string strLoc;
		  strLoc = newLoc +"/"+string(ent->d_name);
	Video video = Video (strLoc.c_str(),true);




	Edge edge = Edge(video);
	vector<float> edgePres = edge.getEdgeVariation();
	float m_max=0, m_min=10000;
	for (int i = 0; i < (int)edgePres.size(); ++i) {
		if(m_max < edgePres.at(i)){
			m_max = edgePres.at(i);
		}
		if(edgePres.at(i) < m_min){
			m_min = edgePres.at(i);
		}
	}
	vector<float> data_1;
	data_1.push_back(m_min);
	data_1.push_back(m_max);
	data.push_back(data_1);

	  }
	  closedir (dir);
	}
	for (int i = 0; i < (int)data.size(); ++i) {
		cout << "[\""<<data.at(i).at(0) << ","  <<  data.at(i).at(1)<< "\"]," << endl;
	}
}

int main(){
	static const int GLOBAL_THRESHOLD = 65, MAX_VALUE = 255;
	static const string PITCH_AVAILABILITY[] = {"Unavailable", "Available"};
	static const string PITCH_ZOOMING[] = {"Not zooming", "Zooming"};
	static const string CAMERA_FOCUSINGS[] = {"Ground", "Sky", "Pavilion"};

	VideoCapture capture;
	Mat capturedFrame, bgrFrame;
	string path;

	cout << "Enter the path of the video file: ";
	cin >> path;

	cout << "Opening video....";
//	capture.open(path);
	cout << "done" << endl << "Extracting data from the video....";

	int numberOfFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);

	int pitchAvailabilities[numberOfFrames];
	double pitchSizes[numberOfFrames];
	double edgePercentages[numberOfFrames];
	int cameraFocusedAreas[numberOfFrames];
	cleanArrays(pitchAvailabilities, pitchSizes, edgePercentages, cameraFocusedAreas, numberOfFrames);

//	for(int i = 0; i < numberOfFrames; i++){
//		capturedFrame = getFrame(capture, 0);
//
//		if(capturedFrame.empty()){
//			break;
//		}

//		cvtColor(capturedFrame, bgrFrame, CV_BGRA2BGR);
//		int width = bgrFrame.cols;
//		int height = bgrFrame.rows;
//
//		int pitchAvailability = 0;
//		double pitchSize = 0.0;

//		pitchAvailability = ImageInfoLoader::isPitchAvailable(bgrFrame, width, height, &pitchSize);
//		pitchAvailabilities[i] = pitchAvailability;
//		pitchSizes[i] = pitchSize;

//		double edgePercentage = ImageInfoLoader::getEdgePercentage(bgrFrame, GLOBAL_THRESHOLD, MAX_VALUE, width, height);
//		edgePercentages[i] = edgePercentage;

//		int cameraFocusedArea = ImageInfoLoader::getCameraFocusedArea(bgrFrame, width, height);
//		cameraFocusedAreas[i] = cameraFocusedArea;

//		capturedFrame.release();
//		bgrFrame.release();
//	}


	getEdgePres(path);
	return 0;
}
