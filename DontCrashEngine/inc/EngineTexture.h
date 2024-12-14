#pragma once
#include <memory>
#include <string>
#include "EngineGraphics.h"

namespace DC_Engine
{
	enum class ApiType;
	class EngineGraphics;
	class Resource;
	class EngineTexture
	{
	public:
		virtual ~EngineTexture() {};

		virtual std::unique_ptr<EngineTexture> Create() = 0;

		virtual bool AddImageToTexture(std::shared_ptr<Resource>) = 0; // add full image

		virtual void* GetNativeTexture(std::string) = 0;

		virtual void Destroy() = 0;
	};

}