#include "GameState.h"
#include <../../DontCrashGame/src/SandBox.h>


namespace DC_Engine
{
    void GameState::RegisterWithLua()
    {
        m_pScriptingManager->CreateTable();

        m_pScriptingManager->CreateTable();
        m_pScriptingManager->AddToTable("GameObjects");

        m_pScriptingManager->SetGlobal("g_sandbox");
    }

    // #DG: this also needs some restructuring. It seems to create "scenes" not "states"
    std::unique_ptr<GameState> DC_Engine::GameState::Create(State state)
    {
        switch (state)
        {
        case DC_Engine::State::None:
            return nullptr;

        case DC_Engine::State::SandBox:
            return std::make_unique<SandBox>();

        default:
            return nullptr;
        }
    }
}