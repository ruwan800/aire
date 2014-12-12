/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */
#include "video.h"
#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include<opencv2/core/core.hpp>


#ifndef IO_H_
#define IO_H_


using namespace std;
using namespace cv;

namespace aire {

class IO {
public:
	IO(string);
	IO(string, Log* log);
	IO(Video);
	virtual ~IO();
	vector<Scalar> readScalarVector(string);
	vector<int> readIntVector(string);
	void write(vector<Scalar>, string);
	void write(vector<string>, string);
	void write(vector<int>, string);
	void setProjectDirectory(string);
	void createDirectory(string);
	void cleanDirectory(string pathname);
	void splitVideoFile(Video video, vector<int> cc, bool createall);
	string createAudioFile(string video_file);
	vector<string> getSubDirs(string folderPath);
	vector<string> getDirFiles(string folderPath);
	string project_dir;
private:
	string aire_dir;
	void writeToFile(string, string);
	void checkDir(string);
	bool isExists(string pathname);
	string getAbsPath(string path);
	vector<String> readFromFile(string);
	Log* LOG;

};

} /* namespace aire */
#endif /* IO_H_ */
