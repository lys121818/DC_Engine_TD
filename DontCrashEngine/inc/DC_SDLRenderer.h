#pragma once
#include "EngineGraphics.h"
#include <memory>
#include <string>

struct SDL_Renderer;
namespace DC_Engine
{
	class EngineTexture;
	class WindowManager;
	enum class ApiType;
	/*==================================================================================

	  DC_SDLRenderer - Engine graphic renderer using SDL_Renderer

	  SDL_Renderer specificly provides in SDL_window

	====================================================================================*/

	class DC_SDLRenderer : public EngineGraphics
	{
		static constexpr const char* imageRefPath = "images/";

	public:
		DC_SDLRenderer();
		~DC_SDLRenderer() override;

		bool Init(std::unique_ptr<WindowManager>& pWindow) override;

		bool StartDraw(const Color& color) override;
		bool StartDraw(const std::string& fileName, const Rect& frameTransform, const Rect& objectTransform) override;
		bool StartDraw(const std::string& fileName, const Rect& frameTransform, const Rect& objectTransform,const double& angle) override;

		bool PreLoadImage(std::shared_ptr<Resource> pResource) override;

		void EndDraw() override;

		void Destroy() override;

	private:
		virtual void CreateTexture(const ApiType& type) override;

	private:
		std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> m_pRenderer;
		std::unique_ptr<EngineTexture> m_pTexture;

	};
}

