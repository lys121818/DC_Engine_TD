#include "DC_SDLRenderer.h"
#include "ApplicationLayer.h"

// Include from Utilities
#include <FileSystem.h>

#include <SDL.h>
#include "WindowManager.h"
#include "DC_SDLTexture2D.h"
#include "DC_Color.h"
#include "DC_Rect.h"
#include "ResourceFile.h"

namespace DC_Engine
{
    DC_SDLRenderer::DC_SDLRenderer()
        :m_pRenderer(nullptr,nullptr)
    {

    }

    DC_SDLRenderer::~DC_SDLRenderer()
    {
        Destroy();
    }


    bool DC_SDLRenderer::Init(std::unique_ptr<WindowManager>& pWindow)
    {
        if (pWindow->GetWindowInfo().platform != ApiType::kSDL)
            return false;

        // Create Renderer using SDL window
        SDL_Window* pSDLWindow = reinterpret_cast<SDL_Window*>(pWindow->GetWindow());

        if (!pSDLWindow)
            return false;

        m_pRenderer = std::unique_ptr<SDL_Renderer, decltype(& SDL_DestroyRenderer)>
            (SDL_CreateRenderer(pSDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),&SDL_DestroyRenderer);

        if (m_pRenderer == nullptr)
            return false;

        SDL_SetRenderDrawBlendMode(m_pRenderer.get(), SDL_BLENDMODE_BLEND); // Set blend mode to support alpha

        // Create Texture
        CreateTexture(ApiType::kSDL);

        return true;
    }

    bool DC_SDLRenderer::StartDraw(const Color& color)
    {
        if (m_pRenderer == nullptr)
            return false;

        SDL_SetRenderDrawColor(m_pRenderer.get(), color.r,color.g,color.b,color.a);

        return true;
    }

    bool DC_SDLRenderer::StartDraw(const std::string& fileName, const Rect& frameTransform, const Rect& objectTransform)
    {
        std::string path = imageRefPath + fileName;

        if (m_pRenderer == nullptr)
            return false;

        SDL_Texture* texture = reinterpret_cast<SDL_Texture*>(m_pTexture->GetNativeTexture(path));

        SDL_Rect imageRect{ (int)frameTransform.position.x, (int)frameTransform.position.y,frameTransform.width,frameTransform.height};
        SDL_Rect objectRect{ (int)objectTransform.position.x, (int)objectTransform.position.y,objectTransform.width, objectTransform.height};

        SDL_RenderCopy(m_pRenderer.get(), texture, &imageRect, &objectRect);

        
        return true;
    }

    bool DC_SDLRenderer::StartDraw(const std::string& fileName, const Rect& frameTransform, const Rect& objectTransform, const double& angle)
    {
        std::string path = imageRefPath + fileName;

        if (m_pRenderer == nullptr)
            return false;

        SDL_Texture* texture = reinterpret_cast<SDL_Texture*>(m_pTexture->GetNativeTexture(path));

        SDL_Rect imageRect{ (int)frameTransform.position.x, (int)frameTransform.position.y,frameTransform.width,frameTransform.height };
        SDL_Rect objectRect{ (int)objectTransform.position.x, (int)objectTransform.position.y,objectTransform.width, objectTransform.height };

        SDL_Point point;

        point.x = objectRect.w / 2;
        point.y = objectRect.h / 2;

        SDL_RenderCopyEx(m_pRenderer.get(), texture, &imageRect, &objectRect, angle, &point, SDL_FLIP_NONE);


        return true;
    }

    bool DC_SDLRenderer::PreLoadImage(std::shared_ptr<Resource> pResource)
    {
        m_pTexture->AddImageToTexture(pResource);
        if (m_pTexture)
            return true;

        return false;
    }

    void DC_SDLRenderer::EndDraw()
    {
        SDL_RenderPresent(m_pRenderer.get());
        SDL_RenderClear(m_pRenderer.get());
    }

    void DC_SDLRenderer::CreateTexture(const ApiType& type)
    {
        if(type == ApiType::kSDL)
            m_pTexture = std::make_unique<DC_SDLTexture2D>(*m_pRenderer);
    }

    void DC_SDLRenderer::Destroy()
    {
        if (m_pRenderer)
            SDL_DestroyRenderer(m_pRenderer.get());
    }

}