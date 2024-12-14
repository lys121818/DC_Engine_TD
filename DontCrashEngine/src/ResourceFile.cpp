#include "ResourceFile.h"

// Include from DontCrashEngine
#include "Resource.h"
#include "ApplicationLayer.h"

// Include from Utilities
#include <FileSystem.h>

// Include from Zlib
#define ZLIB_WINAPI
#include <zlib.h>

// Include from Tinyxml
#include <tinyxml2.h>

// Include from STL
#include <algorithm>
#include <iostream>
#include <filesystem>

namespace DC_Engine 
{
	void ResourceFile::AddResource(std::string filePath, std::vector<char> data)
	{
		YS_Util::FileSystem::NormalizePath(filePath);

		ResourceInfo info;
		info.m_size = static_cast<uint32_t>(data.size());
		info.m_offset = m_currentOffset;

		std::vector<char> compressedData;
		compressedData.resize(data.size());

		z_stream stream = SetStreamBuffer(data, compressedData);

		int result = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
		if (result != Z_OK)
		{
			return;
		}

		result = deflate(&stream, Z_FINISH);

		if (result == Z_STREAM_END && stream.total_out < data.size() && stream.avail_in == 0)
			compressedData.resize(stream.total_out);
		else
			compressedData = std::move(data);

		deflateEnd(&stream);

		info.m_compressed = static_cast<uint32_t>(compressedData.size());
		m_currentOffset += info.m_compressed;
		m_info[filePath] = info;
		m_pendingData.push_back(std::move(compressedData));
	}

	void DC_Engine::ResourceFile::Save(const std::string& filePath)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* pRoot = doc.NewElement("ResourceFile");
		doc.InsertFirstChild(pRoot);

		// save header using XMLdata of resource pack
		for (auto& info : m_info)
		{
			tinyxml2::XMLElement* pElement = doc.NewElement("Resource");
			pElement->SetAttribute("Path", info.first.c_str());
			pElement->SetAttribute("Compressed", info.second.m_compressed);
			pElement->SetAttribute("Size", info.second.m_size);
			pElement->SetAttribute("Offset", info.second.m_offset);
			pRoot->InsertEndChild(pElement);
		}

		tinyxml2::XMLPrinter print;
		doc.Print(&print);

		m_file.open(filePath, std::ios_base::out | std::ios_base::binary);
		if (m_file.is_open())
		{
			for (const auto& data : m_pendingData)
			{
				m_file.write(data.data(), data.size());
			}
			m_file.write(print.CStr(), print.CStrSize());

			int size = print.CStrSize();
			m_file.write(reinterpret_cast<char*>(&size), sizeof(int));

			m_pendingData.clear();
			m_info.clear();
			m_currentOffset = 0;
		}
		m_file.close();
	}

	void ResourceFile::Load(const std::string& filePath)
	{
		std::string basePath = DC_Engine::ApplicationLayer::GetInstance()->GetBasePath() + filePath;
		m_file.open(basePath, std::ios_base::in | std::ios_base::binary);
		if (m_file.is_open())
		{
			int sizeofInt = sizeof(int);
			m_file.seekg(-sizeofInt, m_file.end);

			int headerSize;
			m_file.read(reinterpret_cast<char*>(&headerSize), sizeof(int));
			m_file.seekg(-(headerSize + sizeofInt), m_file.cur);

			std::vector<char> header(headerSize);
			m_file.read(header.data(), headerSize);

			tinyxml2::XMLDocument doc;
			tinyxml2::XMLError error = doc.Parse(header.data(), headerSize);
			if (error != tinyxml2::XML_SUCCESS)
				return;

			// Load using XMLdata of resource pack
			tinyxml2::XMLElement* pRoot = doc.FirstChildElement();
			std::string name(pRoot->Name());
			if (name == "ResourceFile")
			{
				for (tinyxml2::XMLElement* pElement = pRoot->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
				{
					std::string elementName(pElement->Name());
					if (elementName == "Resource")
					{
						ResourceInfo info;
						info.m_compressed = pElement->UnsignedAttribute("Compressed");
						info.m_size = pElement->UnsignedAttribute("Size");
						info.m_offset = pElement->UnsignedAttribute("Offset");
						std::string path = pElement->Attribute("Path");
						if (!path.empty())
						{
							m_info[path] = info;
						}
					}
				}
			}
		}
	}

	std::shared_ptr<Resource> ResourceFile::LoadResource(std::string filePath)
	{
		if (!m_file.is_open())
			return nullptr;

		YS_Util::FileSystem::NormalizePath(filePath);

		// Find Resource
		auto Iter = m_info.find(filePath);
		if (Iter == m_info.end())
			return nullptr;

		// Get Resource data
		std::vector<char> compressed(Iter->second.m_compressed);
		m_file.seekg(Iter->second.m_offset);
		m_file.read(compressed.data(), compressed.size());

		if (Iter->second.m_size == Iter->second.m_compressed)
			return std::make_shared<Resource>(filePath, std::move(compressed));

		// decompress resource data
		std::vector<char> data(Iter->second.m_size);

		z_stream stream = SetStreamBuffer(compressed,data);

		int result = inflateInit(&stream);

		if (result != Z_OK)
			return nullptr;

		result = inflate(&stream, Z_FINISH);
		if (result != Z_STREAM_END)
		{
			inflateEnd(&stream);
			return nullptr;
		}

		inflateEnd(&stream);

		return std::make_shared<Resource>(filePath, std::move(data));
	}

	z_stream ResourceFile::SetStreamBuffer(std::vector<char>& in, std::vector<char>& out)
	{
		z_stream stream;
		memset(&stream, 0, sizeof(stream));
		stream.avail_in = static_cast<uint32_t>(in.size());
		stream.next_in = reinterpret_cast<uint8_t*>(in.data());
		stream.avail_out = static_cast<uint32_t>(out.size());
		stream.next_out = reinterpret_cast<uint8_t*>(out.data());
		return stream;
	}

}