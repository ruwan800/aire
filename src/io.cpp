/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "video.h"
#include "io.h"
#include "objects.h"
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

void IO::splitVideoFile(Video video, vector<int> cc){
	string vfile = (string)video.video_file;
	string dir_name = "video_temp";
	string temp_dir = project_dir+"/"+dir_name;
	char command[100];
	sprintf(command,"rm -rf %s",temp_dir.c_str());
	system( command);
	createDirectory(dir_name);

	int prev = 0;
	int curr;
	for (int i = 1; i < (int)cc.size(); ++i) {
		curr = cc.at(i);
		int len = curr - prev;
		char numb[10];
		sprintf(numb,"%06d.mp4",i);
		char start[20];
		sprintf(start,"%02d:%02d:%02d.%03d",prev/(25*60*60),(prev%(25*60*60))/(25*60),(prev%(25*60))/25,(prev%25)*40);
		char finish[20];
		sprintf(finish,"%02d:%02d:%02d.%03d",len/(25*60*60),(len%(25*60*60))/(25*60),(len%(25*60))/25,(len%25)*40);
		char command[400];
		sprintf(command,"avconv -i %s -ss %s -t %s -acodec copy -vcodec libx264 %s/%s",
						vfile.c_str(),start,finish,temp_dir.c_str(),numb);
		cout << command << endl;//####
		system(command);
		prev = curr;
	}
}

} /* namespace aire */
