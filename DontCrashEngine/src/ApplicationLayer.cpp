#include "ApplicationLayer.h"

// include from standard library
#include <assert.h>
#include <filesystem>
#include <windows.h>
#include <chrono>

// Include from Utility
#include <Timer.h>

// include from engine
#include "WindowConfig.h"
#include "Log.h"

#include "WindowManager.h"
#include "ActionResult.h"
#include "DC_SDLWindow.h"
#include "DC_OpenGLWindow.h"
#include "DC_SDLRenderer.h"
#include "DC_Color.h"
#include "DC_Rect.h"

namespace DC_Engine
{
    ApplicationLayer* ApplicationLayer::s_appLayer = nullptr;
    ActionResult ApplicationLayer::Init()
    {
        s_appLayer = this;

        m_pRng = std::make_unique<RandomNumberGenerator>();

        SetLogger();

        DC_Engine::ActionResult result = DC_Engine::ActionResult::kAPIInitFail;

        // Setting for window configurations
        m_config.platform = DC_Engine::ApiType::kSDL;
        m_config.title = "DontCrashGameEngine";

        // #DG: this should be configurable, in a different location
        m_config.width = 1024;
        m_config.height = 768;




        // Create API
        result = CreateWindowAPI();

        // Log print
        if (result == ActionResult::kSuccess)
        {
            assert(HasAPI());

            m_pLog->Log("API: ", false);

            // #DG: if you give the platform a "GetName()" function you can elminate the need for this if block
            if (m_config.platform == ApiType::kSDL)
                m_pLog->Log("SDL");
            else if (m_config.platform == ApiType::kOpenGL)
                m_pLog->Log("OpenGL");
        }
        else
        {
            m_pLog->LogError("Failed to Create Window");
            return result;
        }


        // Create Renderer
        result = CreateRenderer();

        // Log print
        if (result == ActionResult::kSuccess)
        {
            assert(HasRenderer());

            m_pLog->Log("Renderer: ", false);
        }
        else
        {
            m_pLog->LogError("Failed to Create Renderer");
            return result;
        }


        // Create Event System
        result = CreateInputEvent();
        if (result == ActionResult::kSuccess)
        {
            assert(HasEventSystem());

            m_pLog->Log("Event System is successfully initialized");
        }
        else
            return result;

        // Create Audio
        result = CreateAudio();
        if (result == ActionResult::kSuccess)
        {
            assert(HasAudio());

            if (m_pAudio->Init())
                m_pLog->Log("Successfully Created the Audio");
            else
            {
                m_pLog->LogError("Failed to Initialize Audio");
                return ActionResult::kAPIInitFail;
            }
        }
        else
        {
            m_pLog->LogError("Failed to Created the Audio");
            return result;
        }

        // Create Game App
        m_pGame = CreateGameStateMachine();
        m_pGame->Init();

        // Log print
        if (result == ActionResult::kSuccess)
        {
            assert(HasGame());

            m_pLog->Log("Game: ", false);
            m_pLog->Log(m_pGame->GetName());
        }
        else
        {
            m_pLog->LogError("Failed to Create Game");
            return result;
        }


        return result;
    }

    void ApplicationLayer::Run()
    {
        YS_Util::Timer time;
        m_pRng->SeedFromTime();
        // Todo: delta timer
        while (m_isRunning)
        {
            time.StartTimer();

            Update(time.GetDeltaTime());
            Render();
            EndFrame();

            time.EndTimer();
        }
    }

    void ApplicationLayer::Render()
    {
        m_pGame->Render();
        m_pRenderer->EndDraw();
    }

    std::string ApplicationLayer::GetBasePath()
    {
        char buffer[MAX_PATH];  // Use char for ANSI version
        GetModuleFileNameA(NULL, buffer, MAX_PATH);  // Explicitly call the ANSI version
        std::string fullPath(buffer);
        std::string::size_type pos = fullPath.find_last_of("\\/");
        return fullPath.substr(0, pos);  // Return the directory path
    }

    // Update tick
    void ApplicationLayer::Update(float deltaTime)
    {
        m_isRunning &= m_pWindow->Update(deltaTime);

        m_pGame->Update(deltaTime);

    }

    // Reset
    void ApplicationLayer::EndFrame()
    {
        m_pWindow->NextFrame();
    }


    //////////////
    /// Create  //
    //////////////
    ActionResult ApplicationLayer::CreateWindowAPI()
    {
        ActionResult result = ActionResult::kAPIInitFail;

        m_pWindow = WindowManager::Create(m_config);

        // Successfully create api
        if (m_config.platform != ApiType::kNONE)
        {
            assert(m_pWindow);
            result = ActionResult::kSuccess;

            m_pWindow->Init();
        }

        return result;
    }

    DC_Engine::ActionResult ApplicationLayer::CreateRenderer()
    {
        DC_Engine::ActionResult result = DC_Engine::ActionResult::kAPIInitFail;

        m_pRenderer = DC_Engine::DC_SDLRenderer::Create();

        if (m_pRenderer == nullptr)
            return result;

        if (!m_pRenderer->Init(m_pWindow))
        {
            return result;
        }

        result = DC_Engine::ActionResult::kSuccess;

        return result;
    }

    DC_Engine::ActionResult ApplicationLayer::CreateInputEvent()
    {
        auto pKeyboard = DC_Engine::KeyboardInputEvent::Create();
        if (pKeyboard->Init())
        {
            m_pWindow->AttachKeyboard(std::move(pKeyboard));
        }
        else
            return DC_Engine::ActionResult::kAPIInitFail;

        auto pMouseInput = DC_Engine::MouseInput::Create();
        if (pMouseInput->Init())
        {
            m_pWindow->AttachMouseButton(std::move(pMouseInput));
        }
        else
            return DC_Engine::ActionResult::kAPIInitFail;

        auto pMouseMove = DC_Engine::MouseMove::Create();
        if (pMouseMove->Init())
        {
            m_pWindow->AttachMouseMove(std::move(pMouseMove));
        }
        else
            return DC_Engine::ActionResult::kAPIInitFail;

        return DC_Engine::ActionResult::kSuccess;
    }

    ActionResult ApplicationLayer::CreateAudio()
    {
        m_pAudio = DC_Audio::Create();

        if (m_pAudio)
            return ActionResult::kSuccess;

        return ActionResult::kAPIInitFail;
    }

    void ApplicationLayer::SetLogger()
    {
        m_pLog = std::make_unique<Logger>("Log.log");
        assert(m_pLog);
    }

    ////////////////////////
    /// Check for Possess //
    ////////////////////////
    bool ApplicationLayer::HasAPI() const
    {
        if (m_config.platform == DC_Engine::ApiType::kNONE)
            return false;
        else
        {
            assert(m_pWindow != nullptr);
            return true;
        }
    }
    bool ApplicationLayer::HasGame() const
    {
        if (m_pGame)
            return true;

        return false;
    }
    bool ApplicationLayer::HasRenderer() const
    {
        if (m_pRenderer)
            return true;

        return false;
    }
    bool ApplicationLayer::HasEventSystem() const
    {
        if (m_pWindow->GetKeyboard() && m_pWindow->GetMouseInput() && m_pWindow->GetMouseMove())
            return true;

        return false;
    }

    bool ApplicationLayer::HasAudio() const
    {
        if (m_pAudio)
            return true;
        return false;
    }

    // Free all memory
    void ApplicationLayer::Destroy()
    {
        if (HasAPI())
            m_pWindow->Destroy();
        if (HasGame())
            m_pGame->Destroy();
        if (HasRenderer())
            m_pRenderer->Destroy();

        m_pLog = nullptr;
        m_pGame = nullptr;
        m_pWindow = nullptr;
        m_pRenderer = nullptr;
    }
}