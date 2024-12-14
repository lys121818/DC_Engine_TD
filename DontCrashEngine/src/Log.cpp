#include "Log.h"
#include <fstream>
#include <filesystem>
#include <assert.h>
#include <iostream>

namespace DC_Engine
{

	// Constructor creates the folder and file for logs to be saved
	Logger::Logger(const std::string& fileName)
	{
		// Create folders for log files
		const std::string& folderPath = "Logs";

		if (std::filesystem::create_directory(folderPath))
		{
			std::cout << "Successfully Created the new folder: " << folderPath << std::endl;
		}
		else if (!std::filesystem::exists(folderPath))
		{
			std::cout << "Failed to Create new folder: " << folderPath << std::endl;
		}

	
		// Create log file using the path of folder and filename
		filePath = folderPath + '/' + fileName;
		std::fstream outfile;
		outfile.open(filePath, std::fstream::out);

		assert(outfile.is_open());
		if(!outfile.is_open())
		{
			std::cout << "It has failed to open up the log file." << std::endl;
		}

		outfile.close();
	}

	// Print the log message
	void Logger::Log(const std::string& message, bool isAddLine)
	{
		if (!WriteLog(message, isAddLine))
		{
			std::cout << "It has failed to open up the log file." << std::endl;
		}

	
	}

	void Logger::LogWarning(const std::string& warning, const size_t& warningCode)
	{
		std::cout << "Warning: " << warning << "| WarningCode: " << warningCode << std::endl;
	
		

		WriteLog(warning, warningCode);
	}

	void Logger::LogError(const std::string& error, const size_t& errorcode)
	{
		std::cout << "Error: " << error << "| ErrorCode: " << errorcode << std::endl;

		WriteLog(error, errorcode);
	}

	// Write to log file
	bool Logger::WriteLog(const std::string& message, bool isAddLine, const size_t& code)
	{
		std::fstream outfile;

		outfile.open(filePath, std::ios::app);

		// fail to open the log file
		assert(outfile.is_open());
		if (!outfile.is_open())
		{
			std::cout << "It has failed to open up the log file." << std::endl;
			return false;
		}
	
		outfile << message;

		if (code != 0)
		{
			outfile << "| ErrorCode: ";
			outfile << code;
		}

		if(isAddLine)
			outfile << std::endl;

		outfile.close();

		return true;
	}
}
