#include "FileSystem.h"

// Windows Specific
#include <Windows.h>

// Include from STL-
#include <algorithm>
#include <filesystem>

namespace YS_Util
{
	void FileSystem::NormalizePath(std::string& path)
	{
		// Organize the folder path
		path = std::filesystem::weakly_canonical(path).string();

		// Change all path alphabats to lowercase and change path division to '/'
		transform(path.begin(), path.end(), path.begin(), [](unsigned char c){ return std::tolower(c); });
		replace(path.begin(), path.end(), '\\', '/');
	}
	
	std::vector<std::string> FileSystem::GetAllFiles(const std::string& folderPath)
	{
		std::vector<std::string> filesList;

		GetFilesRecursive(folderPath, "", filesList);

		return filesList;
	}

	std::vector<std::string> FileSystem::GetAllExtensionFiles(const std::string& folderPath, const char* extensionName)
	{
		std::vector<std::string> headerFiles;
		std::vector<size_t> indexToRemove;

		// Get all the files in current working directory
		headerFiles = GetAllFiles(folderPath);


		// Filter only the header files
		for (auto iter = headerFiles.begin(); iter != headerFiles.end();)
		{
			const char* fileType = std::strrchr(iter->c_str(), '.');
			if (fileType)
			{
				fileType++;
				// headerfile
				if (std::strcmp(fileType, extensionName) == 0)
				{
					iter++;
				}
				else
				{
					iter = headerFiles.erase(iter);
				}
			}
			else
			{
				iter = headerFiles.erase(iter);
			}
		}

		return headerFiles;
	}

	void FileSystem::GetFilesRecursive(const std::string& path, const std::string& directory, std::vector<std::string>& files)
	{
		std::string fullPath = path + '/' + directory + "/*";
		WIN32_FIND_DATAA findData;
		HANDLE findHandle = FindFirstFileA(fullPath.c_str(), &findData);

		if (findHandle == INVALID_HANDLE_VALUE)
			return;

		std::string rootDirectory = directory;
		if (!rootDirectory.empty())
			rootDirectory += '/';

		// Recursive until reaches last file
		while (true)
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::string directoryName = findData.cFileName;
				if (directoryName != "." && directoryName != "..")
				{
					std::string nextDirectory = rootDirectory + directoryName;
					GetFilesRecursive(path, nextDirectory, files);
				}
			}
			else
				files.emplace_back(rootDirectory + findData.cFileName);

			bool result = FindNextFileA(findHandle, &findData);
			if (!result)
				break;
		}
	}
}
