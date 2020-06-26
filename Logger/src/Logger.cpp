#include "Logger.h"
#include<iostream>
#include<stdlib.h>
#include<string>
#include<filesystem>
#include<istream>
#include<regex>



namespace nokialg {

	Logger::Logger()
		:
		LastID(0),
		LastErrorID(0),
		LogFileCount(0),
		LogEntryCount(0),
		hConsole (GetStdHandle(STD_OUTPUT_HANDLE))

		
		
	{
		parseConfigFile();
		if (LogToFile) {

			std::string FileName = FileNamePattern + std::to_string(LogFileCount) + ".txt";
			OutputFile.open(LogDirPath + "\\" + FileName,std::ofstream::trunc);
		}
	}

	void Logger::log(LogLevel logLevel, std::string msg, std::string source )
	{
		int ID = generateID(logLevel);
		SetConsoleTextColor(logLevel);

		if (logLevel == ERROR)
		{
			std::string LogEntry = generateLogEntry(logLevel, source, msg, ID, ConsoleLogFormat);
			ActiveErrors.push_back( LogEntry);
		}
		

		if (logLevel >= ConsoleLogLevel && LogToConsole)
		{
			
			
			std::string LogEntry = generateLogEntry(logLevel, source, msg, ID, ConsoleLogFormat);
			std::cout << LogEntry<<std::endl ;
		}
		
		if (logLevel >= FileLogLevel && LogToFile)
		{
			std::string LogEntry = generateLogEntry(logLevel, source, msg, ID, FileLogFormat);
			writeToFile(LogEntry);
		}

	}

	void Logger::getErrors()
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		std::cout <<"\n\n"<< "***************************All active ERRORS********************************" << "\n";
		for (auto& entry : ActiveErrors)
		{
			//if(entry.second!="cleared")
				std::cout << entry<<std::endl;
		}
		std::cout << "****************************************************************************" << "\n\n";
	}

	void Logger::clear(int id)
	{
		try {
			if (abs(id) > ActiveErrors.size() || id > -1)
				throw std::string("clear("+std::to_string(id)+") "+ std::to_string(id) +" is not a valid ERROR ID");
			else
				ActiveErrors.at(std::abs(id) - 1)="cleared"; //mert -1-től kezdődnek az ID-k és minden Error Entrynél eggyel csökken
		}
		catch (const std::string& str) {
			log(ERROR, str); 
		}
	}

	int Logger::generateID(LogLevel logLevel)
	{
		if (logLevel == ERROR)
			return --LastErrorID;
		else
			return ++LastID;
	}

	std::string Logger::generateLogEntry(LogLevel logLevel,std::string source,std::string msg,int id,std::string format)
	{
		std::string LogEntry=format;
		TimePoint tp = getTimeAndDate();
		std::vector<std::regex> reg = { std::regex("\\{year\\}"),std::regex("\\{month\\}"), std::regex("\\{day\\}"), std::regex("\\{hour\\}"), std::regex("\\{min\\}"), std::regex("\\{sec\\}"), std::regex("\\{loglevel\\}"), std::regex("\\{source\\}"), std::regex("\\{msg\\}"), std::regex("\\{id\\}") };
		std::vector<std::string> var = { tp.year,tp.month,tp.day,tp.hour,tp.min,tp.sec,LogLevelToString(logLevel),source,msg,std::to_string(id) };
		

		for (int i = 0; i < reg.size(); i++)
		{
			LogEntry = std::regex_replace(LogEntry, reg[i], var[i]);
		}

		return LogEntry;
	}

	TimePoint Logger::getTimeAndDate()
	{
		auto Now = std::chrono::system_clock::now();
		std::time_t Time = std::chrono::system_clock::to_time_t(Now);
		tm Tm_date = *localtime(&Time);
		TimePoint Date = tmToTimePoint(Tm_date);
		

		return Date;

	}

	TimePoint Logger::tmToTimePoint(const tm& tm)
	{
		TimePoint Now;
		Now.year = std::to_string(tm.tm_year+1900);
		Now.month = std::to_string(tm.tm_mon+1);
		Now.day = std::to_string(tm.tm_mday);
		Now.hour= std::to_string(tm.tm_hour);
		Now.min = std::to_string(tm.tm_min);
		Now.sec = std::to_string(tm.tm_sec);

		return Now;
	}

	void Logger::writeToFile(std::string str)
	{
		if (LogEntryCount == MaxNumOfEntries)
		{
			LogEntryCount = 0;
			LogFileCount++;
			if (LogFileCount != MaxNumOfFiles&&LogRotation)
			{
				OutputFile.close();
				std::string FileName = FileNamePattern + std::to_string(LogFileCount) + ".txt"; //itt a logot kellene helyettesíteni csak
				OutputFile.open(LogDirPath + "\\" + FileName, std::ofstream::trunc);
				LogEntryCount++;
				OutputFile << str << std::endl;

			}
			else
			{
				std::cout << "[!!!!] You run out of the available space, which was predefined in the config file so further log entries will not be written to file [!!!!]" << std::endl;
				LogToFile = false;
			}

		}
		else {

			LogEntryCount++;
			OutputFile << str << std::endl;

		}
	}

	void Logger::SetConsoleTextColor(LogLevel logLevel)
	{
		switch (logLevel)
		{
		case nokialg::DEBUG: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			break;
		case nokialg::INFO: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			break;
		case nokialg::WARNING: SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_GREEN);
			break;
		case nokialg::ERROR: SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			break;
		
		}
	}

	void Logger::parseConfigFile() //lehetne commenteket rakni a config fájlba
	{
		std::ifstream ConfigFile;
		ConfigFile.open(ConfigFilePath);
		std::string nextline;
		std::string key;
		std::string value;

		while (std::getline(ConfigFile, nextline)) //Configparameters feltöltése
		{
			//nextline.erase(std::remove_if(nextline.begin(), nextline.end(), [](char c) {return c == '\t'||c==' '; }), nextline.end());
			std::stringstream ss;
			ss << nextline;
			std::getline(ss, key, '=');
			std::getline(ss, value);
			ConfigParameters[key] = value;
			
		}

		std::map<std::string, LogLevel> MapLevels = { {"WARNING",WARNING},{"INFO",INFO},{"DEBUG",DEBUG},{"ERROR",ERROR} };

		LogToConsole = ConfigParameters["LogToConsole"] == "TRUE" ? true : false;
		ConsoleLogLevel = MapLevels[ConfigParameters["ConsoleLogLevel"]];
		ConsoleLogFormat= ConfigParameters["ConsoleLogFormat"];

		LogToFile = ConfigParameters["LogToFile"] == "TRUE" ? true : false;
		FileLogLevel = MapLevels[ConfigParameters["FileLogLevel"]];
		FileLogFormat = ConfigParameters["FileLogFormat"];

		LogRotation= ConfigParameters["LogRotation"] == "TRUE" ? true : false;
		MaxNumOfEntries = std::stoi(ConfigParameters["MaxNumOfEntries"]);
		MaxNumOfFiles= std::stoi(ConfigParameters["MaxNumOfFiles"]);

		LogDirPath = ConfigParameters["LogDirPath"];
		ConfigFilePath = ConfigParameters["ConfigFilePath"];
		FileNamePattern = ConfigParameters["FileNamePattern"];
		


	}

	std::string LogLevelToString(LogLevel logLevel)
	{
		switch (logLevel)
		{
		case nokialg::DEBUG:	return "DEBUG";
			break;
		case nokialg::INFO:		return "INFO";
			break;
		case nokialg::WARNING:	return "WARNING";
			break;
		case nokialg::ERROR:	return "ERROR";
			break;

		}
	}

	Logger::~Logger()
	{
		OutputFile.close();
	}
}