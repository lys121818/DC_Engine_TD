#pragma once

// Include from STL
#include <string>
#include <vector>

namespace YS_Util
{
	// File Utility System
	class FileSystem
	{
	public:
		static void NormalizePath(std::string& path);
		/*
		*	- Organize the folder path
		* 
		*	- Change all path alphabats to lowercase and change path division to '/'
		*/

		/*	This is window specific */
		static std::vector<std::string> GetAllFiles(const std::string& folderPath);
		/*
		*	Store all the files in the path to vector of string
		*/
		// Todo: have this work in multiple platform

		static std::vector<std::string> GetAllExtensionFiles(const std::string& folderPath, const char* extensionName);
		/*
		*	Store all the files with extension Name in the path to vector of string
		*/

	private:
		static void GetFilesRecursive(const std::string& path, const std::string& directory, std::vector<std::string>& files);
		/*
		*	Recursive call function if next file exist
		*/
	};
}

