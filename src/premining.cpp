/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "premining.h"

using namespace std;
using namespace cv;

namespace aire {



PreMining::PreMining() {
}

PreMining::~PreMining() {
	// TODO Auto-generated destructor stub
}
vector<Scalar> PreMining::processCameraMotion(vector<Scalar> cm, vector<int> cc){
	vector<Scalar> output;
	for (unsigned int i = 1; i < cc.size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			vector<Scalar> sub = vector<Scalar>(cm.begin()+cc.at(i-1)+(j/3*i),cm.begin()+cc.at(i)+((j+1)/3*i));
			Scalar meanV, stdDV;
			meanStdDev(sub,meanV, stdDV);
			int width = (cc[i]-cc[i-1])/3;
			output.push_back(Scalar(width, width, width));
			output.push_back(meanV);
			output.push_back(stdDV);
		}
	}
	return output;
}


} /* namespace aire */
