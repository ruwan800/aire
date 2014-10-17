/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "video.h"


#ifndef IO_H_
#define IO_H_

using namespace std;

namespace aire {

class IO {
public:
	IO(Video);
	virtual ~IO();
	void writeToFile(vector<int>, string);
private:
	string workspace;
	void createWorkspace();
	string formatPath(const char*);



};

} /* namespace aire */
#endif /* IO_H_ */
