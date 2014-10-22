/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include <string>

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
	void e(string, string);
	void d(string, string);
	void i(string, string);
	void w(string, string);
private:
	struct message {
	  string m_type;
	  string m_category;
	  string m_value;
	};
	std::vector<message> m_log;
	void addToLog(message);
	void print(string, string, string);
	void process(string, string, string);


};

} /* namespace aire */
#endif /* LOG_H_ */
