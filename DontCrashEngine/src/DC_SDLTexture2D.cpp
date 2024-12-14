#include "ApplicationLayer.h"
#include "DC_SDLTexture2D.h"
#include "SDL_render.h"
#include "SDL_image.h"
#include "Log.h"
#include "Resource.h"
#include <assert.h>

#include <iostream>
namespace DC_Engine
{
    DC_SDLTexture2D::~DC_SDLTexture2D()
    {
        Destroy();
    }

    std::unique_ptr<EngineTexture> DC_SDLTexture2D::Create()
    {
        return std::unique_ptr<EngineTexture>();
    }

    bool DC_SDLTexture2D::AddImageToTexture(std::shared_ptr<Resource> pResource)
    {
        // Existing Image
        if (m_pTextures.find(pResource->GetName()) != m_pTextures.end())
        {
            ApplicationLayer::GetInstance()->GetLog().Log("The Image is already added: " + pResource->GetName());
            return false;
        }

        // Use SDL_RWops to create a surface from the image data in memory
        SDL_RWops* rw = SDL_RWFromMem(pResource->GetData().data(), static_cast<int>(pResource->GetData().size()));
        if (!rw)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to create RWops for image: " + pResource->GetName());
            return false;
        }


        SDL_Surface* imageSurface = IMG_Load_RW(rw,1);  // 1 to close rw after use

        if (!imageSurface)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to Load Image: " + pResource->GetName());
            SDL_RWclose(rw);
            return false;
        }
        assert(imageSurface);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(&m_pOwner, imageSurface);

        assert(texture);
        if (!texture)
        {
            ApplicationLayer::GetInstance()->GetLog().LogError("Failed to Load Texture: " + pResource->GetName());
            return false;
        }

        // Add texture file to map
        m_pTextures.insert({ pResource->GetName(), texture });

        // Clean up
        SDL_FreeSurface(imageSurface);
        imageSurface = nullptr;

        return true;
    }

    void* DC_SDLTexture2D::GetNativeTexture(std::string fileName)
    {
        if (m_pTextures[fileName] == nullptr)
            return nullptr;

        return reinterpret_cast<void*>(m_pTextures[fileName]);

    }

    void DC_SDLTexture2D::Destroy()
    {
        for (auto iter = m_pTextures.begin(); iter != m_pTextures.end(); ++iter)
        {
            SDL_DestroyTexture(iter->second);
            iter->second = nullptr;
        }
    }


}