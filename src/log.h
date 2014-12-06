/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include <string>
#include <iostream>
#include <vector>

#ifndef LOG_H_
#define LOG_H_

using namespace std;

namespace aire {




class Log {

public:
	Log();
	virtual ~Log();
	void e(char*, char*);
	void d(char*, char*);
	void i(char*, char*);
	void w(char*, char*);
	void e(const string, string);
	void d(const string, string);
	void i(const string, string);
	void w(const string, string);
	void setEnabled(bool enabled);
	struct Process {
		string name;
		int progress;
		int boundary;
		void setProcessBoundary(int boundary){
			this->boundary = boundary;
			//std::cout << "~~~~~~~~~~~setting project boundary::" << boundary << std::endl;//####
		}
		void setProcessProgress(int progress){
			this->progress = progress;
			int progressval = this->progress*100/this->boundary;
			//std::cout << "project progress pppp::" << progressval << std::endl;//####
			if(progressval%10 == 0){
				std::cout << "project progress::" << progressval << "%" << std::endl;//####
			}
		}
	};
	Process* startProcess(string process_name);
	void endProcess(Process m_process);
	Process getMainProcess();
	Process getSubProcess();
private:
	struct message {
	  string m_type;
	  string m_category;
	  string m_value;
	};
	bool log_enabled;
	std::vector<message> m_log;
	void addToLog(message);
	void print(string, string, string);
	void processMessage(string, string, string);
	std::vector<Process> processes;


};

} /* namespace aire */
#endif /* LOG_H_ */
