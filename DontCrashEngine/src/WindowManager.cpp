#include "WindowManager.h"

#include "DC_SDLWindow.h"
#include "DC_OpenGLWindow.h"

namespace DC_Engine
{

	std::unique_ptr<WindowManager> DC_Engine::WindowManager::Create(const WindowConfig& config)
	{
		switch (config.platform)
		{
		case ApiType::kNONE:
		{
			return nullptr;
		}
		case ApiType::kSDL:
		{
			return std::make_unique<DC_SDLWindow>(config);
		}
		case ApiType::kOpenGL:
		{
			return std::make_unique<DC_OpenGLWindow>(config);
		}
		default:
			break;
		}

		return nullptr;
	}
}

