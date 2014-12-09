/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "edge.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

namespace aire {

Log::Log() {
	log_enabled = true;
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


Log::Process* Log::startProcess(string process_name){
	for (unsigned int i = 0; i < processes.size(); ++i) {	//####
		this->i(processes.at(i).name+"::@old", format("%d/%d",processes.at(i).progress,processes.at(i).boundary));
		//cout << "SP0::" << processes.at(i).name << "::" << processes.at(i).progress << "::" << processes.at(i).boundary << "::" << endl;			//####
	}														//####
	for (unsigned int i = 0; i < processes.size(); ++i) {
		if(processes.at(i).name.compare(process_name) == 0 ){
			return &processes.at(0);
		}
	}
	Process pr = Process();
	pr.name = process_name;
	processes.push_back(pr);
	for (unsigned int i = 0; i < processes.size(); ++i) {	//####
		this->i(processes.at(i).name+"::@new", format("%d/%d",processes.at(i).progress,processes.at(i).boundary));
		//cout << "SP1::" << processes.at(i).name << "::" << processes.at(i).progress << "::" << processes.at(i).boundary << "::" << endl;
	}														//####
	return &processes.back();
}
void  Log::endProcess(Process m_process){
	for (unsigned int i = 0; i < processes.size(); ++i) {	//####
		this->i(processes.at(i).name+"::@old", format("%d/%d",processes.at(i).progress,processes.at(i).boundary));
		//cout << "SE0::" << processes.at(i).name << "::" << processes.at(i).progress << "::" << processes.at(i).boundary << "::" << endl;
	}														//####
	while(true){
		if(! processes.size()){
			break;
		}
		if(processes.back().name.compare(m_process.name) == 0 ){
			processes.pop_back();
			break;
		}
		processes.pop_back();
	}
	for (unsigned int i = 0; i < processes.size(); ++i) {	//####
		this->i(processes.at(i).name+"::@new", format("%d/%d",processes.at(i).progress,processes.at(i).boundary));
		//cout << "SE1::" << processes.at(i).name << "::" << processes.at(i).progress << "::" << processes.at(i).boundary << "::" << endl;
	}														//####

}

Log::Process Log::getMainProcess(){
	if(1 <= processes.size()){
		return processes.front();
	}
	else{
		Process p = Process();
		p.name = "Main Process Complete";
		p.boundary = -1;
		p.boundary = -1;
		return p;
	}
}

Log::Process Log::getSubProcess(){
	if(2 <= processes.size()){
		return processes.at(1);
	}
	else{
		Process p = Process();
		p.name = "Main Process Complete";
		p.boundary = -1;
		p.boundary = -1;
		return p;
	}
}


} /* namespace aire */
