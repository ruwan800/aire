/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "edge.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

namespace aire {

Edge::Edge(Video vid)
	:video(vid)
{
	video = vid;
	LOG = vid.LOG;
}

Edge::~Edge() {
	// TODO Auto-generated destructor stub
}

std::vector<float> Edge::getEdgeVariation() {

	Mat gray,blured,canny;
	int ratio = 3;
	int kernel_size = 3;
	int lowThreshold = 50;
	for (int i = 0; i < (int)video.size(); ++i) {

		Mat src = video.getFrame(i);
		cvtColor( src, gray, CV_BGR2GRAY );
		/// Reduce noise with a kernel 3x3
		blur( gray, blured, Size(3,3) );

		/// Canny detector
		Canny( blured, canny, lowThreshold, lowThreshold*ratio, kernel_size );
		float mean_value = mean(canny)[0];
		edge_changes.push_back(mean_value);
		char output[200];
		sprintf(output,"Edge Mean: %5.2f",mean_value);
		LOG.i("output",string(output));
		//imshow("ssss", canny);
		//cvWaitKey(0);
	}
	return edge_changes;
}

std::vector<std::vector<float> > Edge::getSectionEdgeVariation() {

	Mat gray,blured,canny;
	int ratio = 3;
	int kernel_size = 3;
	int lowThreshold = 50;
	for (int i = 0; i < (int)video.size(); ++i) {

		Mat src = video.getFrame(i);
		std::vector<float> sections;
		cvtColor( src, gray, CV_BGR2GRAY );
		/// Reduce noise with a kernel 3x3
		blur( gray, blured, Size(3,3) );

		/// Canny detector
		Canny( blured, canny, lowThreshold, lowThreshold*ratio, kernel_size );
		for (int j = 0; j < 8; ++j) {
			int sw = canny.rows/8;
			Mat nc1 = canny.rowRange(j*sw,j*sw+sw);
			float edge_values[4];
			for (int k = 0; k < 4; ++k) {
				int sc = canny.cols/4;
				Mat nc2 = nc1.colRange(k*sc,k*sc+sc);
				float mean_value = mean(nc2)[0];
				sections.push_back(mean_value);
				edge_values[k] = mean_value;
				//cout << mean_value << ",    ";//####
			}
			char output[200];
			sprintf(output,"%5.2f    %5.2f    %5.2f    %5.2f",edge_values[0],edge_values[1],edge_values[2],edge_values[3]);
			LOG.i(string("output"),string(output));
		}
		LOG.i("output",string(""));

		section_edge_changes.push_back(sections);
	}
	return section_edge_changes;
}

} /* namespace aire */
