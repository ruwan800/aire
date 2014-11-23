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


PreMining::PreMining(IO t_io) {
	io = t_io;
	cc = io.readIntVector("camera_changes.txt");
	cm = io.readScalarVector("camera_motion.txt");
}

PreMining::PreMining(string str) {
	io = IO(str);
	cc = io.readIntVector("camera_changes.txt");
	cm = io.readScalarVector("camera_motion.txt");
}

PreMining::~PreMining() {
	// TODO Auto-generated destructor stub
}
vector<Scalar> PreMining::processCameraMotion(){
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


vector<int> * PreMining::splitPoints(vector<int>* points){
	for (int i = 0; i < 3; ++i) {
		int lastpoint = 0;
		for ( int j = 0; j < (int) cm.size(); ++j) {
			if(j < lastpoint)continue;
			int value = 0;
			for ( int k = 0; k < (int) cm.size()-j; ++k) {
				value = (value*k+cm.at(j)[i])/(k+1);
				if( 5 < abs(value-cm.at(j+k)[i])){
					lastpoint = j+k;
					if(k<25)break;
					if(	j+k < cc.at(0) && 10 <= j && 10 <= cc.at(0)-(j+k)){
						points[i].push_back(j);
						points[i].push_back(j+k);
						break;
					}
					if(	cc.back() < j && 10 <= j-cc.back() && 10 <= cm.size()-(j+k)){
						points[i].push_back(j);
						points[i].push_back(j+k);
						break;
					}
					for (int l = 1; l < (int)cc.size(); ++l) {
						if(	cc.at(l-1) < j && j+k < cc.at(l) &&
							10 <= j-cc.at(l-1) && 10 <= cc.at(l)-(j+k)){
							points[i].push_back(j);
							points[i].push_back(j+k);
							break;
						}
					}
					break;
				}
			}
		}
	}
	return points;
}

vector<vector<string> > PreMining::findCameraMotionClass(){
	int previous = 0;
	int current;
	vector<vector<vector<int> > > regionset;
	for ( int i = 0; i < (int) cc.size()+1; ++i) {
		vector<int> m_regions;
		for (int k = 0; k < 5; ++k) {
			m_regions.push_back(0);
		}
		vector<vector<int> > regions;
		for (int j = 0; j < 3; ++j) {
			regions.push_back(m_regions);
		}
		current = (i != (int) cc.size()) ? cc.at(i) : cm.size();
		for ( int j = previous; j < current; ++j) {
			for (int k = 0; k < 3; ++k) {
				int val = cm.at(j)[k];
				if(val <= -10){
					regions.at(k).at(0) ++;
					regions.at(k).at(1) ++;
				}
				else if ( -10 < val && val <= -2){
					regions.at(k).at(1) ++;
				}
				else if ( -2 < val && val <= 1){
					regions.at(k).at(2) ++;
				}
				else if ( 1 < val && val <= 9){
					regions.at(k).at(3) ++;
				}
				else if ( 9 < val){
					regions.at(k).at(3) ++;
					regions.at(k).at(4) ++;
				}
			}
		}

		regionset.push_back(regions);

		if(i != (int) cc.size()) previous = (int) cc.at(i);
	}

	vector<vector<string> > result;
	vector<string> m_result;
	for (int i = 0; i < 3; ++i) {
		result.push_back(m_result);
	}
	for (int i = 0; i < (int)regionset.size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			string val;
			int count = 0;
			if(10 <= regionset.at(i).at(j).at(4) && 10 <= regionset.at(i).at(j).at(0)){
				val += "+-HL";
			}
			else if(50 <= regionset.at(i).at(j).at(4)){
				val += "+HL";
			}
			else if(50 <= regionset.at(i).at(j).at(0) ){
				val += "-HL";
			}
			else if(50 <= regionset.at(i).at(j).at(2) ){
				val += "0L";
			}
			else if(50 <= regionset.at(i).at(j).at(3) ){
				val += "+LL";
				count += 2;
			}
			else if(50 <= regionset.at(i).at(j).at(1) ){
				val += "-LL";
				count += 2;
			}
			else if(10 <= regionset.at(i).at(j).at(4) ){
				val += "+HS";
			}
			else if(10 <= regionset.at(i).at(j).at(0) ){
				val += "-HS";
			}
			else if(10 <= regionset.at(i).at(j).at(3) && 10 <= regionset.at(i).at(j).at(1) ){
				val += "+-LS";
				count += 2;
			}
			else if(10 <= regionset.at(i).at(j).at(3) && regionset.at(i).at(j).at(3) < 50 && count == 0){
				val += "+LS";
				count ++;
			}
			else if(10 <= regionset.at(i).at(j).at(1) && regionset.at(i).at(j).at(1) < 50 && count == 0){
				val += "-LS";
				count ++;
			}
			else if(10 <= regionset.at(i).at(j).at(2) && regionset.at(i).at(j).at(2) < 50 && count == 0){
				val += "0S";
				count ++;
			}
			else if(count == 0){
				val += "ANY";
			}
			result[j].push_back(val);
		}
	}
	return result;
}


} /* namespace aire */
