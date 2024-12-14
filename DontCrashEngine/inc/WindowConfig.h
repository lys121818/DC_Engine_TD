#pragma once
#include <string>

namespace DC_Engine
{
	enum class ApiType
	{
		kNONE = 0,
		kSDL = 1,
		kOpenGL,
	};

	struct WindowConfig
	{
	public:
		// Construct
		WindowConfig() = default;
		WindowConfig(const WindowConfig&) = default;
		WindowConfig(WindowConfig&&) = default;
		WindowConfig(const ApiType& type, std::string&& title, const size_t& width, const size_t& height)
			: platform(type), title(title), width(width), height(height) {}

		// Assign
		WindowConfig& operator=(const WindowConfig&) = default;
		WindowConfig& operator=(WindowConfig&&) = default;

		ApiType platform = ApiType::kNONE;
		size_t width = 1024;
		size_t height = 768;
		std::string title = "Untitled";


		const WindowConfig& get() const { return *this; }
	};
}