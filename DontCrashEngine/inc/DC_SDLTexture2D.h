#pragma once
#include "EngineTexture.h"

#include <unordered_map>
#include <memory>


struct SDL_Texture;
struct SDL_Renderer;
namespace DC_Engine
{
	class DC_SDLTexture2D : public EngineTexture
	{
	public:
		DC_SDLTexture2D(SDL_Renderer& pRenderer) :m_pOwner(pRenderer) {};
		~DC_SDLTexture2D() override;

		std::unique_ptr<EngineTexture> Create() override;

		bool AddImageToTexture(std::shared_ptr<Resource> pResource) override;

		void* GetNativeTexture(std::string fileName) override;

		void Destroy() override;

	private:
		std::unordered_map<std::string, SDL_Texture*> m_pTextures;
		SDL_Renderer& m_pOwner;
		std::string m_fileName;


	};
}
