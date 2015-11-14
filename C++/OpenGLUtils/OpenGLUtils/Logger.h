#pragma once

#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#define GL_UTILS_LOG_ENABLED

using namespace std;

class Logger
{
private:
	static Logger* defaultLogger;

	static const string LEVELS[3];

	static const int L_INFO;
	static const int L_WARN;
	static const int L_ERR;

	std::ostream * os;
	Logger(ostream * os);
public:
	static Logger* getDefaultLogger();

	void writeMessage(const int level, const string &scope, const string &message);
	void writeMessageStream(const int level, const string &scope, stringstream &stream);

	void SetOutputStream(ostream * os);

	~Logger(void);
};

