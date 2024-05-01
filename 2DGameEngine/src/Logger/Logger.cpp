#include "./Logger.h"
#include <string>
#include <chrono>
#include <ctime>
#include <iostream>

std::string CurrentDateTimeToString() {

	// this function from the course triggers an error for using localtime
	/*std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	std::strftime(&output[0], output.size(), "%d-%b-%y %H:%M:%S", std::localtime(&now));
	return output;*/

	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm tm_snapshot;
	char buffer[30]; // Temp buffer to hold the datetime string.

	// Use localtime_s instead of localtime.
	localtime_s(&tm_snapshot, &now);

	// Write the time and date to the buffer
	std::strftime(buffer, sizeof(buffer), "%d-%b-%y %H:%M:%S", &tm_snapshot);

	return std::string(buffer); // Constructing string from buffer
}

// static member variables are expected to be defined as well
std::vector<LogEntry> Logger::messages;
void Logger::Log(const std::string& message) {
	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = "LOG: [" + CurrentDateTimeToString() + "] " + message;
	std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
	messages.push_back(logEntry);
}
void Logger::Err(const std::string& message) {
	LogEntry logEntry;
	logEntry.type = LOG_ERROR;
	logEntry.message = "ERR: [" + CurrentDateTimeToString() + "] " + message;
	std::cout << "\x1B[91m" << logEntry.message<< "\033[0m" << std::endl;
	messages.push_back(logEntry);
}