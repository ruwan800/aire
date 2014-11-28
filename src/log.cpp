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
}

Log::~Log() {
	// TODO Auto-generated destructor stub
}

void Log::addToLog(message msg) {
	m_log.push_back(msg);
}

void Log::print(string t_type, string t_cat, string t_val) {
	cout << t_type << " :: " << t_cat << " :: " << t_val << endl ;
}


void Log::process(string t_type, string t_cat, string t_val ){
	message msg;
	msg.m_type = t_type;
	msg.m_category = t_cat;
	msg.m_value = t_val;
	addToLog(msg);
	print(t_type, t_cat, t_val );

}

void Log::e(char* cat, char* val){
	char x[] = "e";
	process(x, cat, val );
}

void Log::d(char* cat, char* val){
	char x[] = "d";
	process(x, cat, val );
}

void Log::i(char* cat, char* val){
	char x[] = "i";
	process(x, cat, val );
}

void Log::w(char* cat, char* val){
	char x[] = "w";
	process(x, cat, val );
}

void Log::e(string cat, string val){
	string x = "e";
	process(x, cat, val );
}

void Log::d(string cat, string val){
	string x = "d";
	process(x, cat, val );
}

void Log::i(string cat, string val){
	string x = "i";
	process(x, cat, val );
}

void Log::w(string cat, string val){
	string x = "w";
	process(x, cat, val );
}

} /* namespace aire */
