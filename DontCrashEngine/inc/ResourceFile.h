#pragma once

// Include from STL
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

// Forward declate
struct z_stream_s;
typedef struct z_stream_s z_stream;
namespace DC_Engine
{
	// Forward declate
	class Resource;
	class ResourceFile
	{
	// struct
	private:
		struct ResourceInfo
		{
			uint32_t m_compressed;
			uint32_t m_size;	// uncompressed size
			uint32_t m_offset;
		};

	// Functions
	public:
		ResourceFile() : m_currentOffset(0) {}

		void AddResource(std::string filePath, std::vector<char> data);
		void Save(const std::string& filePath);	// Save resource information to XML file
		void Load(const std::string& filePath); // Load resource information using XML file

		std::shared_ptr<Resource> LoadResource(std::string filePath);

	private:
		z_stream SetStreamBuffer(std::vector<char>& in, std::vector<char>& out); // Set stream to compress or decompress

	// Variables
	private:
		std::unordered_map<std::string, ResourceInfo> m_info;

		uint32_t m_currentOffset;
		std::vector<std::vector<char>> m_pendingData;
		std::fstream m_file;
	};
}

