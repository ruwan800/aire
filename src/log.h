/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */



#ifndef LOG_H_
#define LOG_H_

namespace aire {

class Log {
public:
	Log();
	virtual ~Log();
	void e(char*, char*);
	void d(char*, char*);
	void i(char*, char*);
	void w(char*, char*);
private:
	struct message {
	  char* m_type;
	  char* m_category;
	  char* m_value;
	};
	std::vector<message> m_log;
	void addToLog(message);
	void print(char*, char*, char*);
	void process(char*, char*, char*);


};

} /* namespace aire */
#endif /* LOG_H_ */
