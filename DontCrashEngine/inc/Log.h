#pragma once

#include <string>

namespace DC_Engine
{

	/*=================================================================================================================================

	  Logger -	Logger class creates the '.log' file to save the log data into human readable txt file.
				This class creates the folder call 'Logs' if the folder does not exist.
				Inside the folder, it creates the log file with specific file name of user choice or default name with 'temp.log'
	=================================================================================================================================*/

	class Logger
	{
	private:
		std::string filePath;

	public:

		/* Logger should not be copyable but moveable. */
		// - Multi instances writing to the same file should not happen especially when it's using multithreading
		// - Logger class can be moveable to transfer the ownership without duplicate
		Logger(const std::string& fileName = "temp.log");	// default file name with 'temp.log'
		Logger(const Logger&) = delete; // no copy constructor
		Logger(Logger&&) = default; // allow move constructor
		~Logger() {};


		// Print the log message
		void Log(const std::string& message, bool isAddLine = true);
		void LogWarning(const std::string& warning, const size_t& warningCode = 999);
		void LogError(const std::string& error, const size_t& errorcode = 999);

	private:
		// Write to log file
		bool WriteLog(const std::string& message, bool isAddLine = true, const size_t& code = 0);
	};
}