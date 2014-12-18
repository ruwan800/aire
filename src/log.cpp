/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: aire
 */

#include "edge.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

namespace aire {

Log::Log() {
	log_enabled = true;
	mainProcess.boundary = 0;
	mainProcess.progress = 0;
	subProcess.boundary = 0;
	subProcess.progress = 0;
}

Log::~Log() {
	// TODO Auto-generated destructor stub
}

void Log::addToLog(message msg) {
	m_log.push_back(msg);
}

void Log::print(string t_type, string t_cat, string t_val) {
	if(log_enabled){
		cout << t_type << " :: " << t_cat << " :: " << t_val << endl ;
	}
}

void Log::setEnabled(bool enabled){
	log_enabled = enabled;
}


void Log::processMessage(string t_type, string t_cat, string t_val ){
	message msg;
	msg.m_type = t_type;
	msg.m_category = t_cat;
	msg.m_value = t_val;
	addToLog(msg);
	print(t_type, t_cat, t_val );

}

void Log::e(char* cat, char* val){
	char x[] = "e";
	processMessage(x, cat, val );
}

void Log::d(char* cat, char* val){
	char x[] = "d";
	processMessage(x, cat, val );
}

void Log::i(char* cat, char* val){
	char x[] = "i";
	processMessage(x, cat, val );
}

void Log::w(char* cat, char* val){
	char x[] = "w";
	processMessage(x, cat, val );
}

void Log::e(const string cat, string val){
	char x[] = "e";
	processMessage(x, cat, val );
}

void Log::d(const string cat, string val){
	char x[] = "d";
	processMessage(x, cat, val );
}

void Log::i(const string cat, string val){
	char x[] = "i";
	processMessage(x, cat, val );
}

void Log::w(const string cat, string val){
	char x[] = "w";
	processMessage(x, cat, val );
}

void Log::startMainProcess(string name){
	mainProcess.name = name;
}

void Log::startSubProcess(string name){
	subProcess.name = name;
}

void Log::endMainProcess(){
	mainProcess.name = "";
	mainProcess.boundary = 0;
	mainProcess.progress = 0;
}
void Log::endSubProcess(){
	subProcess.name = "";
	subProcess.boundary = 0;
	subProcess.progress = 0;
}

int Log::getMainProcessBoundary(){
	return mainProcess.boundary;
}
int Log::getSubProcessBoundary(){
	return subProcess.boundary;
}
int Log::getMainProcessProgress(){
	return mainProcess.progress;
}
int Log::getSubProcessProgress(){
	return subProcess.progress;
}

void Log::setMainProcessBoundary(int boundary){
	mainProcess.boundary = boundary;
}

void Log::setSubProcessBoundary(int boundary){
	subProcess.boundary = boundary;
}

void Log::incrementMainProcessBoundary(int increment){
	mainProcess.boundary += increment;
}

void Log::incrementSubProcessBoundary(int increment){
	subProcess.boundary += increment;
}

void Log::setMainProcessProgress(int progress){
	mainProcess.progress = progress;
	if(progress%10 == 0){
		std::cout << "project progress::" << progress << std::endl;//####
		//Log::i("::progress", "");
	}
}

void Log::setSubProcessProgress(int progress){
	subProcess.progress = progress;
	if(progress%10 == 0){
		std::cout << "project progress::" << progress/25/60 << ":" << (progress/25)%60 << std::endl;//####
		//Log::i("::progress", "");
	}
}

void Log::incrementMainProcess(string name){
	mainProcess.progress ++;
	if(mainProcess.progress%10 == 0){
		std::cout << "project progress::" << mainProcess.progress << std::endl;//####
		//Log::i("::progress", "");
	}
	mainProcess.name = name;
	endSubProcess();

}

void Log::incrementSubProcess(){
	subProcess.progress ++;
	if(mainProcess.progress%10 == 0){
		std::cout << "project progress::" << mainProcess.progress << std::endl;//####
		//Log::i("::progress", "");
	}
}

void Log::setMainProcess(string name){
	mainProcess.name = name;
}


} /* namespace aire */
