#pragma once
#include<iostream>
#include<vector>
#include<chrono>
#include<ctime>
#include<fstream>
#include<map>
#include<sstream>
#include<Windows.h>
#pragma warning(disable : 4996)
/*
NokiaLoggingTask

Write a logging library in Java or C++ language.
Log levels are: DEBUG, INFO, WARNING, ERROR in this order.
Required features:
- Provide a log() function with message, log level and source parameters (source can be an exception class in case of ERRORs - so it is an optional parameter)
- Provide a getErrors() function to list all of the active ERROR log entries
- Log entries should have an auto generated unique ID
- Provide a clear() function that expects an ID and sets the corresponding ERROR log entry to be "cleared". It is not listed in the getErrors() anymore. 
If the ERROR could not be found by the ID, raise an exception and log an ERROR about it
- Make your service externally configurable
	- It can be a config file with a predefined structure
	- It can be a constructor injection
	- etc.
- Configurable settings:
	- Enable/disable logging to console
	- Set logging format in case of console logging (date-time, message, level)
	- Set the log level in case of console logging (e.g. if log level is DEBUG we will log all of the logs, in case of WARNING, we log the WARNING and the ERROR logs)
	- Enable/disable logging to file
	- Give the filename or filename pattern (e.g. log{0..}.txt means log0.txt, log1.txt, log2.txt, etc.)
	- Set a log rotation (so when a log file is "full" another log file starts)
	- Describe the condition of a log file treated as full
	- Set the number of log files in the rotation (the log.txt should be the active one, and when it is full,
	should be renamed to log1.txt and a new log.txt should be created)
	- Set the logging format in case of file logging
	- Set the log level in case of file logging
- Create a short documentation about the functions and config rules
- Write clean, well-organized code, create simple API, try to be as OOP as you can
*/
#undef ERROR 
namespace nokialg {

	enum  LogLevel { DEBUG, INFO, WARNING, ERROR };

	std::string LogLevelToString(LogLevel logLevel);

	struct TimePoint {
		std::string year;
		std::string month;
		std::string day;
		std::string hour;
		std::string min;
		std::string sec;
	};
	

	class Logger{ 
	public:
		#pragma region Interface
		
		Logger();

		void log(LogLevel logLevel, std::string msg, std::string source="Invalid ID exception");
		void getErrors();
		void clear(int id);

		~Logger();
	
		#pragma endregion

	private:
		
	#pragma region Private_Members
		//a Formatokat még ki kell találni hogy enum legyen-e
		#pragma region Configuration_Paramters 

		std::map<std::string, std::string> ConfigParameters; //Ide olvasom be a config.cfg-ből az összetartozó értékeket

		/*Változók amiket majd a ConfigParameters segítségével beállítok*/
		bool			LogToConsole;
		std::string		ConsoleLogFormat;
		LogLevel		ConsoleLogLevel;
		bool			LogToFile;
		std::string		FileNamePattern;
		bool			LogRotation;
		int				MaxNumOfEntries; //fileonként
		int				MaxNumOfFiles;	 //futtatásonként
		std::string		FileLogFormat;
		LogLevel		FileLogLevel;
		std::string		LogDirPath;		//ide kerülnek majd a logfájlok

		#pragma endregion  

		#pragma region Variables_for_Filemanipulation

		std::ofstream OutputFile; 
		std::string ConfigFilePath = "C:\\nokialg\\config.txt";
		unsigned int LogFileCount;
		unsigned int LogEntryCount;

		#pragma endregion

		#pragma region Logging_Variables
		
		std::vector<std::string> ActiveErrors;
		int LastID;
		int LastErrorID;

		HANDLE hConsole;
		
		#pragma endregion

	#pragma endregion

	#pragma region Private_Methods
	
		void parseConfigFile(); //beolvassa a config.cfg file-t és feltölti a megfelő adattagokat

		int generateID(LogLevel logLevel); 
		TimePoint tmToTimePoint(const tm& tm); //csak átcastolja az inteket stringgé hogy utána egyszerűbb legyen használni
		TimePoint getTimeAndDate();
		std::string generateLogEntry(LogLevel logLevel,std::string source,std::string msg,int id,std::string format); //meg kéne adni hogy milyen legok vannak amikkel játszhatnak aztán egy loopban összerakni az entryt
		void writeToFile(std::string str);
		void SetConsoleTextColor(LogLevel logLevel);

	#pragma endregion

	};

	



}