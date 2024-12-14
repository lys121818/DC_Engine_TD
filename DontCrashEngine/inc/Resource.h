#pragma once

// Include from STL
#include <string>
#include <vector>


namespace DC_Engine
{
	class Resource
	{
	public:
		Resource(const std::string& name, std::vector<char> data)
			: m_name(name), m_data(std::move(data)) {}

		const std::string& GetName() const { return m_name; }
		std::vector<char>& GetData() { return m_data; }



	private:
		std::string m_name;
		std::vector<char> m_data;
	};
}