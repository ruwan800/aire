/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: aire
 */

#include "video.h"
#include "io.h"
#include "objects.h"
#include "log.h"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

namespace aire {

IO::IO(string str) {
	project_dir = str.substr(0,str.rfind("."));
	Log log = Log();
	LOG = &log;
}

IO::IO(string str, Log* log)
	:LOG(log)
{
	project_dir = str.substr(0,str.rfind("."));
}

IO::IO(Video vid)
	:LOG(vid.LOG)
{
	aire_dir = "/tmp/aire/";
	//string vf = (string)vid.video_file;
	//vf = vf.substr(vf.rfind("/"));
	//vf = vf.substr(0,vf.rfind("."));
	//project_dir= aire_dir+vf;
	string str = (string)vid.video_file;
	project_dir = str.substr(0,str.rfind("."));
}

IO::~IO() {
	// TODO Auto-generated destructor stub
}

void IO::checkDir(string pathname){
	struct stat info;
	if( stat( project_dir.c_str(), &info ) != 0 ){
		LOG->d("create_directory",project_dir);
		mkdir(project_dir.c_str(), 0777);
	}
	string fullpath  = getAbsPath(pathname);
	struct stat info1;
	if( stat( fullpath.c_str(), &info1 ) != 0 ){
		LOG->d("create_directory",fullpath);
		mkdir(fullpath.c_str(), 0777);
	}
}

bool IO::isExists(string pathname){
	string fullpath = getAbsPath(pathname);
	struct stat info;
	if( stat( fullpath.c_str(), &info ) != 0 ){
		return false;
	}
	return true;
}

void IO::createDirectory(string pathname){
	struct stat info;
	string real_dir = getAbsPath(pathname);
	if( stat( real_dir.c_str(), &info ) != 0 ){
		LOG->d("create_directory",real_dir);
		mkdir(real_dir.c_str(), 0777);
	}
}

void IO::cleanDirectory(string pathname){
	string temp_dir = getAbsPath(pathname);
	char command[100];
	sprintf(command,"rm -rf %s",temp_dir.c_str());
	system(command);
}

string IO::getAbsPath(string path){
	if(path[0] == '/' || path[0] == '~'){
		unsigned int found=path.find(project_dir);
		if (found!=string::npos){
			string str = path;
			string str1 = path;
			unsigned int newpos = 0;
			unsigned int pos = 0;
			while(true){
				pos = str.find(string("/"));
				if (pos == string::npos) break;
				newpos += pos+1;
				struct stat info;
				str = str.substr (pos+1);
				str1 = path.substr (0, newpos);
				if( stat( str.c_str(), &info ) != 0 ){
					struct stat info;
					if( stat( str1.c_str(), &info ) != 0 ){
						LOG->d("create_directory",str1);
						mkdir(str1.c_str(), 0777);
					}
				}
			}
		}
		return path;
	}
	checkDir(project_dir);
	return project_dir+"/"+path;
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

void IO::write(vector<string> data, string filename){
	stringstream content;
	for (int i = 0; i < (int) data.size(); ++i) {
		content << data.at(i) << "\n";
	}
	writeToFile(filename, content.str());
}

void IO::writeToFile(string filename, string content ){
	if(filename.find("/") != std::string::npos){
		LOG->d("write",filename);
		ofstream f;
		f.open((filename).c_str(), std::fstream::out);
		f << content;
		f.close();
	}
	else{
		LOG->d("write",project_dir+"/"+filename);
		ofstream f;
		f.open((project_dir+"/"+filename).c_str(), std::fstream::out);
		f << content;
		f.close();
	}
}

vector<String> IO::readFromFile(string filename){
	LOG->d("read",project_dir+"/"+filename);
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

void IO::splitVideoFile(Video video, vector<int> cc, bool createall){
	if(!createall && cc.size() == 2){
		LOG->i("IO",string("no camera changes:: ")+video.video_file);
		return;
	}
	string vfile = (string)video.video_file;
	string dir_name = "video_temp";
	string temp_dir = project_dir+"/"+dir_name;
	cleanDirectory(dir_name);
	createDirectory(dir_name);
	int fr = video.getFrameRate();

	int prev = 0;
	int curr;
	for (int i = 1; i < (int)cc.size(); ++i) {
		curr = cc.at(i);
		int len = curr - prev;
		char numb[20];
		sprintf(numb,"%06d.mp4",i);
		char start[20];
		sprintf(start,"%02d:%02d:%02d.%03d",prev/(fr*60*60),(prev%(fr*60*60))/(fr*60),(prev%(fr*60))/fr,(prev%fr)*(1000/fr));
		char finish[20];
		sprintf(finish,"%02d:%02d:%02d.%03d",len/(fr*60*60),(len%(fr*60*60))/(fr*60),(len%(fr*60))/fr,(len%fr)*(1000/fr));
		char command[400];
		sprintf(command,"avconv -i %s -ss %s -t %s -acodec copy -vcodec libx264 %s/%s",
						vfile.c_str(),start,finish,temp_dir.c_str(),numb);
		cout << command << endl;//####
		system(command);
		prev = curr;
	}
}

string IO::createAudioFile(string video_file){
	string dir_name = "audio";
	string temp_dir = project_dir+"/"+dir_name;
	string audio_file = temp_dir+"/audio.wav";
	if(isExists(audio_file)) return audio_file;
	createDirectory(temp_dir);
	char command[1000];
	sprintf(command,"avconv -i \"%s\"  \"%s\"",video_file.c_str(),audio_file.c_str());
	system(command);
	LOG->i(string("audio"), command);
	//delete[] command;
	return audio_file;
}

vector<string> IO::getSubDirs(string folderPath){
	vector<string> dirs;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (folderPath.c_str())) != NULL) {
	  /* print all the files and directories within directory */

		while ((ent = readdir (dir)) != NULL) {
			struct stat st;
			lstat(ent->d_name, &st);
			if(string(ent->d_name).length() < 3){
				continue;
			}
			unsigned int found=string(ent->d_name).find(".");
			if (found==std::string::npos){
				dirs.push_back(folderPath +"/"+string(ent->d_name) );
			}
		}
	}
	sort(dirs.begin(), dirs.end());
	return dirs;
}

vector<string> IO::getDirFiles(string folderPath){
	vector<string> dirs;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (folderPath.c_str())) != NULL) {
	  /* print all the files and directories within directory */

		while ((ent = readdir (dir)) != NULL) {
			string dirfile = ent->d_name;
			if(dirfile.length() < 3){
				continue;
			}
			struct stat st;
			lstat(dirfile.c_str(), &st);
			/*if(! S_ISDIR(st.st_mode)){
				continue;
			}*/
			unsigned int found=dirfile.find(".");
			if (found==std::string::npos){
				continue;
			}
			dirs.push_back(folderPath +"/"+string(ent->d_name) );
		}
	}
	sort(dirs.begin(), dirs.end());
	return dirs;
}


} /* namespace aire */
