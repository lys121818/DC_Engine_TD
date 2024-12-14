#pragma once
#include <memory>
#include <string>

namespace DC_Engine
{
	struct Rect;
	struct Color;
	
	class WindowManager;
	class Resource;

	enum class ApiType;

	class EngineGraphics
	{
	public:
		virtual ~EngineGraphics() {};

		virtual bool Init(std::unique_ptr<WindowManager>&) = 0;
		virtual bool StartDraw(const Color&) = 0;	// Draw with color
		virtual bool StartDraw(const std::string&, const Rect&, const Rect&) = 0;	// Draw with image file
		virtual bool StartDraw(const std::string& fileName, const Rect& frameTransform, const Rect& objectTransform, const double& angle) = 0;
		virtual void EndDraw() = 0;
		virtual bool PreLoadImage(std::shared_ptr<Resource>) = 0;

		virtual void Destroy() = 0;

		static std::unique_ptr<EngineGraphics> Create();

	private:
		virtual void CreateTexture(const ApiType&) = 0;
	};
}

