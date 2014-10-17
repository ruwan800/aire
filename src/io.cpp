/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;

namespace aire {

IO::IO(Video vid) {
	workspace = "/tmp/aire/"+formatPath(vid.video_file);
}

IO::~IO() {
	// TODO Auto-generated destructor stub
}

void IO::writeToFile(vector<int> input, string outfile) {
	ofstream ofs(workspace+outfile);
	for(int i = 0; i < input.size() ; i++) {

	    ofs << input.at(i) << endl;
	}
	ofs.close();
}

string IO::formatPath(const char* path){
	//strip insane characters.... :D
	return (String)path;
}

} /* namespace aire */
