/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */


#include<opencv2/core/core.hpp>

#ifndef PREMINING_H_
#define PREMINING_H_


using namespace std;
using namespace cv;

namespace aire {

class PreMining {
public:
	PreMining();
	virtual ~PreMining();
	vector<Scalar> processCameraMotion(vector<Scalar>, vector<int>);
private:

};

} /* namespace aire */
#endif /* PREMINING_H_ */
