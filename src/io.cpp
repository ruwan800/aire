/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "video.h"
#include "io.h"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include<opencv2/core/core.hpp>

using namespace std;
using namespace cv;

namespace aire {

IO::IO() {
}

IO::IO(string str) {
	project_dir = str.substr(0,str.rfind("."));
	LOG = Log();
	checkDir(project_dir);
}

IO::IO(Video vid) {
	aire_dir = "/tmp/aire/";
	//string vf = (string)vid.video_file;
	//vf = vf.substr(vf.rfind("/"));
	//vf = vf.substr(0,vf.rfind("."));
	//project_dir= aire_dir+vf;
	string str = (string)vid.video_file;
	project_dir = str.substr(0,str.rfind("."));
	LOG = Log();
	checkDir(project_dir);
}

IO::~IO() {
	// TODO Auto-generated destructor stub
}

void IO::checkDir(string pathname){
	struct stat info;
	if( stat( pathname.c_str(), &info ) != 0 ){
		LOG.d("create_directory",pathname);
		mkdir(pathname.c_str(), 0777);
	}
}

void IO::createDirectory(string pathname){
	struct stat info;
	string real_dir = project_dir+"/"+pathname;
	if( stat( real_dir.c_str(), &info ) != 0 ){
		LOG.d("create_directory",real_dir);
		mkdir(real_dir.c_str(), 0777);
	}
}

vector<Scalar> IO::readScalarVector(string filename){
	vector<Scalar> result;
	vector<String> input = readFromFile(filename);
	for (int i = 0; i < (int) input.size(); ++i) {
		string str = input.at(i);
		Scalar scalar;
		for (int i = 0; i < 4; ++i) {
			int pos = str.length();
			if(i!=3) pos = str.find(",");
			scalar[i] = atof(str.substr(0, pos).c_str());
			if(i!=3) str = str.substr(pos+1);
		}
		result.push_back(scalar);
	}
	return result;
}

vector<int> IO::readIntVector(string filename){
	vector<String> input = readFromFile(filename);
	vector<int> output;
	for (int i = 0; i < (int) input.size(); ++i) {
		output.push_back(atoi(input.at(i).c_str()));
	}
	return output;
}


void IO::write(vector<Scalar> data, string filename){
	stringstream content;
	for (int i = 0; i < (int) data.size(); ++i) {
		content << data.at(i)[0] << ","<< data.at(i)[1] << ","<< data.at(i)[2] << ","<< data.at(i)[3] << "\n";
	}
	writeToFile(filename, content.str());
}

void IO::write(vector<int> data, string filename){
	stringstream content;
	for (int i = 0; i < (int) data.size(); ++i) {
		content << data.at(i) << "\n";
	}
	writeToFile(filename, content.str());
}


void IO::writeToFile(string filename, string content ){
	LOG.d("write",project_dir+"/"+filename);
	ofstream f;
	f.open((project_dir+"/"+filename).c_str(), std::fstream::out);
	f << content;
	f.close();
}

vector<String> IO::readFromFile(string filename){
	LOG.d("read",project_dir+"/"+filename);
	vector<String> result;
	ifstream file((project_dir+"/"+filename).c_str());
	if(file.is_open()){
		while(! file.eof()){
			string cont;
			file >> cont;
			result.push_back(cont);
		}
	}
	result.pop_back();
	return result;
}
} /* namespace aire */
