#pragma once
#include <GameState.h>

// Include from DontCrashEngine
#include <ObjectFactory.h>

// Include from STL
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>


// Forward declare
namespace DC_Engine
{
	class GameObject;
	class EngineGraphics;
	class DC_Audio;
}
class EnemyView;
class PlayerView;

class SandBox : public DC_Engine::GameState
{
	static constexpr size_t s_kPossibleViewCounts = 10;
// Functions
public:
	SandBox();
	~SandBox() override;

	bool Init() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;
	void DestoryGameObject(uint32_t id) override;
	void AddView(std::unique_ptr<DC_Engine::DC_View> pView, ViewId viewId) override;
	


	static std::unique_ptr<DC_Engine::DC_ObjectComponent> CreateTransformComponent(DC_Engine::GameObject* pOwner, std::string name);
	static std::unique_ptr<DC_Engine::DC_ObjectComponent> CreateImageRenderComponent(DC_Engine::GameObject* pOwner, std::string name);
	static std::unique_ptr<DC_Engine::DC_ObjectComponent> CreateSoundComponent(DC_Engine::GameObject* pOwner, std::string name);
	static std::unique_ptr<DC_Engine::DC_ObjectComponent> CreateTriggerBoxComponent(DC_Engine::GameObject* pOwner, std::string name);
	static std::unique_ptr<DC_Engine::DC_ObjectComponent> CreateEventActionComponent(DC_Engine::GameObject* pOwner, std::string name);

private:
	// Initialize
	void InitGame();
	void InitObjectFactory();
	bool LoadTextures();

	void CreateGameObject(DC_Engine::Event* pEvent);
	void SpawnBulletObject(DC_Engine::Event* pEvent);
	void DestroyObject(DC_Engine::Event* pEvent);
	void PlayMusic(DC_Engine::Event* pEvent);
	void PlaySound(DC_Engine::Event* pEvent);
	void ButtonPressed(DC_Engine::Event* pEvent);
	void EndGame(DC_Engine::Event*);

	// Handling Events
	bool HandleEvent(float deltaTime);

	// Lua
	void RegisterWithLua();


// Variables
public:

private: 

	// Interface
	DC_Engine::EngineGraphics& m_pRenderer;
	DC_Engine::DC_Audio& m_pBackgroundMusic;

	float m_xGravity = 0;
	float m_yGravity = 0;
};