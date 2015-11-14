#include "Logger.h"

Logger* Logger::defaultLogger;

const string Logger::LEVELS[3] = { "Info", "Warn", "Err" };

const int Logger::L_INFO = 0;
const int Logger::L_WARN = 1;
const int Logger::L_ERR = 2;

Logger::Logger(ostream * os)
{
	this->os = os;
}


Logger::~Logger(void)
{
}

Logger* Logger::getDefaultLogger() {
	if(defaultLogger == NULL)
		defaultLogger = new Logger(&(std::cout));
	return defaultLogger;
}

void Logger::writeMessage(const int level, const string &scope, const string &message) {
	*(this->os) << Logger::LEVELS[level] << " ->\t" <<  scope << ": " << message << endl;
}

void Logger::writeMessageStream(const int level, const string &scope, stringstream &stream) {
	std::string line;
	while(!stream.eof()) {
		getline(stream, line);
		writeMessage(level, scope, line);
	}
}

void Logger::SetOutputStream(ostream * os) {
	this->os = os;
}
