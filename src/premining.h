/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: aire
 */


#include<opencv2/core/core.hpp>
#include "io.h"

#ifndef PREMINING_H_
#define PREMINING_H_


using namespace std;
using namespace cv;

namespace aire {

class PreMining {
public:
	PreMining();
	PreMining(IO);
	PreMining(string);
	virtual ~PreMining();
	vector<Scalar> processCameraMotion();
	vector<int> * splitPoints(vector<int>*);
	vector<vector<int> > findCameraMotionClass(vector<int> cc, vector<Scalar> cm);

private:
	IO io;
	vector<int> cc;
	vector<Scalar> cm;

};

} /* namespace aire */
#endif /* PREMINING_H_ */
