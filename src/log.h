/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: aire
 */

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

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
	};
	int getMainProcessBoundary();
	int getSubProcessBoundary();
	int getMainProcessProgress();
	int getSubProcessProgress();
	void startMainProcess(string name);
	void startSubProcess(string name);
	void endMainProcess();
	void endSubProcess();
	void setMainProcessBoundary(int boundary);
	void setSubProcessBoundary(int boundary);
	void setMainProcessProgress(int boundary);
	void setSubProcessProgress(int boundary);
	void incrementMainProcessBoundary(int increment);
	void incrementSubProcessBoundary(int increment);
	void incrementMainProcess();
	void incrementSubProcess();
	Process mainProcess;
	Process subProcess;
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


};

} /* namespace aire */
#endif /* LOG_H_ */
