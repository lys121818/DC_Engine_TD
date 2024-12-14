#include "SandBox.h"

// included from DonCrashEngine
#include <ApplicationLayer.h>
#include <GameObject.h>
#include <Input.h>
#include <DC_ObjectComponent.h>
#include <GameStateMachine.h>
#include <EventManager.h>
#include <DC_PhysicsManager.h>
#include <Level.h>

// <Components>
#include <TransformComponent.h>
#include <Box2DPhysics.h>
#include <Box2DStaticBody.h>
#include <Box2DDynamicBody.h>
#include <ImageRenderComponent.h>
#include <SoundComponent.h>
#include <TriggerBoxComponent.h>
#include <EventActionComponent.h>

// Included from DontCrashGame
#include "PlayerView.h"
#include "GameView.h"
#include "EnemyView.h"
#include "BulletView.h"
#include "EndZone.h"


#include "CreateGameObjectEvent.h"
#include "SpawnBulletEvent.h"
#include "DestroyGameObjectEvent.h"
#include "ButtonPressedEvent.h"
#include "PlayMusicEvent.h"
#include "EndGameEvent.h"
#include "PlaySoundEvent.h"

// Included from SDL
#include <functional>


SandBox::SandBox()
	:
	m_pRenderer(DC_Engine::ApplicationLayer::GetInstance()->GetGraphics()),
	m_pBackgroundMusic(DC_Engine::ApplicationLayer::GetInstance()->GetAudio()),
	m_xGravity(0),
	m_yGravity(0)
{

}

SandBox::~SandBox()
{
	Exit();
}

namespace Lua
{
	static int CreateGameObject(lua_State* pState)
	{
		const char* pXML = luaL_checkstring(pState, 1);

		auto& pGameState = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState();
		auto& objectFactory = pGameState.GetObjectFactory();
		auto pResource = pGameState.GetResourceFile().LoadResource(pXML);
		auto pGameObject = objectFactory.CreateGameObject(pResource);
		auto& gameObjects = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetGameObjects();
		gameObjects[pGameObject->GetId()] = pGameObject;

		lua_pop(pState, 1);

		// add our object to our GameObject table
		lua_getglobal(pState, "g_sandbox");
		lua_getfield(pState, -1, "GameObjects");
		lua_newtable(pState);
		lua_rawseti(pState, -2, pGameObject->GetId());
		lua_pop(pState, 2);
		
		pGameObject->RegisterWithScript();

		lua_pushinteger(pState, pGameObject->GetId());
		return 1;
	}

	static int CreatePlayerView(lua_State* pState)
	{
		DC_Engine::GameObject::Id actorId = static_cast<DC_Engine::GameObject::Id>(
			luaL_checkinteger(pState, 1));
		lua_pop(pState, 1);

		auto& pGameState = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState();
		auto pGameObjects = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetGameObjects();
		auto pGameObject = pGameObjects[actorId];
		pGameObject->SetType(DC_Engine::ObjectType::kPlayer);

		auto pView = std::make_unique<PlayerView>(DC_Engine::kPlayerViewId);
		pView->SetPlayer(pGameObject);
		pView->Init();

		pGameState.AddView(move(pView),pGameObject->GetId());
		return 0;
	}

	static int CreateEnemyView(lua_State* pState)
	{
		DC_Engine::GameObject::Id actorId = static_cast<DC_Engine::GameObject::Id>(
			luaL_checkinteger(pState, 1));
		lua_pop(pState, 1);

		auto& pGameState = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState();
		auto pGameObjects = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetGameObjects();
		auto pGameObject = pGameObjects[actorId];
		pGameObject->SetType(DC_Engine::ObjectType::kEnemy);

		auto pView = std::make_unique<EnemyView>(DC_Engine::kEnemyViewId);
		pView->SetEnemy(pGameObject);
		pView->SetTargetType(pGameObjects[0].get());
		pView->Init();

		pGameState.AddView(std::move(pView), pGameObject->GetId());
		return 0;
	}


	static int CreateEndView(lua_State* pState)
	{
		DC_Engine::GameObject::Id actorId = static_cast<DC_Engine::GameObject::Id>(
			luaL_checkinteger(pState, 1));
		lua_pop(pState, 1);

		auto& pGameState = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState();
		auto pGameObjects = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetGameObjects();
		auto pGameObject = pGameObjects[actorId];

		auto pView = std::make_unique<EndZone>(DC_Engine::kEndGameId);
		pView->Init();

		pGameState.AddView(std::move(pView), pGameObject->GetId());
		return 0;
	}

	static int CreateGameView(lua_State* pState)
	{
		DC_Engine::GameObject::Id actorId = static_cast<DC_Engine::GameObject::Id>(
			luaL_checkinteger(pState, 1));
		lua_pop(pState, 1);

		auto& pGameState = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState();
		auto pGameObjects = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState().GetGameObjects();
		auto pGameObject = pGameObjects[actorId];
		pGameObject->SetType(DC_Engine::ObjectType::kNone);

		auto pView = std::make_unique<GameView>(DC_Engine::kGameViewId);
		pView->SetGameObject(pGameObject);
		pView->Init();

		pGameState.AddView(std::move(pView), pGameObject->GetId());
		return 0;
	}

}


bool SandBox::Init()
{
	// Get Instace log
	auto& log = DC_Engine::ApplicationLayer::GetInstance()->GetLog();

	// Create and Initialize the Game Control Managers
	m_pEventManager = std::make_unique<DC_Engine::EventManager>();
	m_pScriptingManager = std::make_unique<DC_Engine::ScriptingManager>();
	m_pResouceFile = std::make_unique<DC_Engine::ResourceFile>();
	m_pObjectFactory = std::make_unique<DC_Engine::ObjectFactory>();
	m_pPhysicsManager = DC_Engine::DC_PhysicsManager::Create(DC_Engine::DC_PhysicsManager::Type::kBox2D, m_xGravity, m_yGravity);


	m_pResouceFile->Load("/resources.bin");

	m_pViews.reserve(s_kPossibleViewCounts);


	if (m_pPhysicsManager->Init() == false)
	{
		log.LogError("Unable to Initialize the PhysicsManager");
		return false;
	}

	if (m_pScriptingManager->Init() == false)
	{
		log.LogError("Unable to Initialize the ScriptingManager");
		return false;
	}

	RegisterWithLua();
	// Initialize Game settings
	DC_Engine::Level level;
	auto pLevelObject = level.LoadTiles(m_pResouceFile.get(), "TileSet/Stage_01.tmx");
	m_pGameObjects[pLevelObject->GetId()] = pLevelObject;
	InitGame();

	auto pScript = m_pResouceFile->LoadResource("Scripts/Script.lua");
	m_pScriptingManager->RunScript(pScript);


	return true;
}

void SandBox::Update(float deltaTime)
{
	HandleEvent(deltaTime);
	m_pEventManager->ProcessEvents();
	m_pPhysicsManager->Update(deltaTime);

	for (auto& gameObject : m_pGameObjects)
		gameObject.second->Update(deltaTime);

}

void SandBox::Render()
{
	for (auto& object : m_pGameObjects)
		object.second->Render(&m_pRenderer);

}


bool SandBox::LoadTextures()
{
	bool isLoaded = false;
	isLoaded = m_pRenderer.PreLoadImage(m_pResouceFile->LoadResource("Images/Buttons.png"));

	if (!isLoaded)
		return false;

	return true;
}

bool SandBox::HandleEvent(float deltaTime)
{
	for (auto& view : m_pViews)
		view.second->UpdateInput(deltaTime);

	return true;
}

void SandBox::RegisterWithLua()
{
	GameState::RegisterWithLua();

	m_pScriptingManager->GetGlobal("g_sandbox");
	m_pScriptingManager->AddToTable("CreateGameObject", Lua::CreateGameObject);
	m_pScriptingManager->AddToTable("CreatePlayerView", Lua::CreatePlayerView);
	m_pScriptingManager->AddToTable("CreateEnemyView", Lua::CreateEnemyView);
	//m_pScriptingManager->AddToTable("SpawnBullet", Lua::SpawnBullet);
	m_pScriptingManager->AddToTable("CreateEndView", Lua::CreateEndView);
	m_pScriptingManager->AddToTable("CreateGameView", Lua::CreateGameView);

	m_pScriptingManager->PopAll();
}




void SandBox::InitGame()
{
	InitObjectFactory();
	LoadTextures();


	// Add Event Listener
	m_pEventManager->AddEventListener(CreateGameObjectEvent::kEventType,
		std::bind(&SandBox::CreateGameObject, this, std::placeholders::_1));

	m_pEventManager->AddEventListener(SpawnBulletEvent::kEventType,
		std::bind(&SandBox::SpawnBulletObject,this,std::placeholders::_1));

	m_pEventManager->AddEventListener(SpawnBulletEvent::kEventType,
		std::bind(&SandBox::DestroyObject, this, std::placeholders::_1));

	m_pEventManager->AddEventListener(PlayMusicEvent::kEventType,
		std::bind(&SandBox::PlayMusic, this, std::placeholders::_1));

	m_pEventManager->AddEventListener(PlaySoundEvent::kEventType,
		std::bind(&SandBox::PlaySound, this, std::placeholders::_1));

	m_pEventManager->AddEventListener(ButtonPressedEvent::kEventType,
		std::bind(&SandBox::ButtonPressed, this, std::placeholders::_1));

	m_pEventManager->AddEventListener(EndGameEvent::kEventType,
		std::bind(&SandBox::EndGame, this, std::placeholders::_1));

	// Player
	m_pEventManager->QueueEvent(std::make_unique<CreateGameObjectEvent>("GameObjects/MusicPlayButton.xml", DC_Engine::kGameViewId));
	m_pEventManager->QueueEvent(std::make_unique<CreateGameObjectEvent>("GameObjects/EnemySpawnButton.xml", DC_Engine::kGameViewId));
	m_pEventManager->QueueEvent(std::make_unique<CreateGameObjectEvent>("GameObjects/ExitButton.xml", DC_Engine::kGameViewId));

}

void SandBox::InitObjectFactory()
{
	m_pObjectFactory->RegisterComponentCreator("TransformComponent", &CreateTransformComponent);
	m_pObjectFactory->RegisterComponentCreator("StaticBodyComponent",
		[this](DC_Engine::GameObject* pOwner, std::string name) -> std::unique_ptr<DC_Engine::DC_StaticBodyComponent> {
			return m_pPhysicsManager->CreateStaticBodyComponent(pOwner, name.c_str());
		});
	m_pObjectFactory->RegisterComponentCreator("DynamicBodyComponent",
		[this](DC_Engine::GameObject* pOwner, std::string name) -> std::unique_ptr<DC_Engine::DC_DynamicBodyComponent> {
			return m_pPhysicsManager->CreateDynamicBodyComponent(pOwner, name.c_str());
		});
	m_pObjectFactory->RegisterComponentCreator("ImageRenderComponent", &CreateImageRenderComponent);
	m_pObjectFactory->RegisterComponentCreator("SoundComponent", &CreateSoundComponent);
	m_pObjectFactory->RegisterComponentCreator("TriggerBoxComponent", &CreateTriggerBoxComponent);
	m_pObjectFactory->RegisterComponentCreator("EventActionComponent", &CreateEventActionComponent);
}

void SandBox::CreateGameObject(DC_Engine::Event* pEvent)
{
	auto pCreateGameObject = static_cast<CreateGameObjectEvent*>(pEvent);

	auto pResource = m_pResouceFile->LoadResource(pCreateGameObject->GetXMLFile());

	auto pGameObject = m_pObjectFactory->CreateGameObject(pResource);
	DC_Engine::DC_View* pView = nullptr;

	switch (pCreateGameObject->GetViewType())
	{
		case DC_Engine::kPlayerViewId:
		{
			auto pPlayerView = new PlayerView(pCreateGameObject->GetViewType());
			pGameObject->SetType(DC_Engine::ObjectType::kPlayer);
			pPlayerView->SetPlayer(pGameObject);
			pView = pPlayerView;
			break;
		}
		case DC_Engine::kEnemyViewId:
		{
			auto pEnemyView = new EnemyView(pCreateGameObject->GetViewType());
			pGameObject->SetType(DC_Engine::ObjectType::kEnemy);
			pEnemyView->SetEnemy(pGameObject);
			pEnemyView->SetTargetType(m_pGameObjects[0].get());
			pView = pEnemyView;
			break;
		}
		case DC_Engine::kGameViewId:
		{
			auto pGameView = new GameView(pCreateGameObject->GetViewType());
			pGameObject->SetType(DC_Engine::ObjectType::kNone);
			pGameView->SetGameObject(pGameObject);
			pView = pGameView;
			break;
		}
		case DC_Engine::ViewType::kEnd:
		{
			auto pGameView = new EndZone(pCreateGameObject->GetViewType());
			pGameView->SetGameObject(pGameObject);
			pView = pGameView;
			break;
		}
		case DC_Engine::ViewType::kNone:
		{
			return;
		}
	default:
		break;
	}
	pView->Init();
	AddView(std::unique_ptr<DC_Engine::DC_View>(pView),pGameObject->GetId());
	m_pGameObjects[pGameObject->GetId()] = pGameObject;

}

void SandBox::SpawnBulletObject(DC_Engine::Event* pEvent)
{
	auto pCreateBulletObject = static_cast<SpawnBulletEvent*>(pEvent);

	auto pResource = m_pResouceFile->LoadResource(pCreateBulletObject->GetXMLFile());

	auto pGameOject = m_pObjectFactory->CreateGameObject(pResource);

	TransformComponent* bulletTransform = static_cast<TransformComponent*>(pGameOject->GetComponent(DC_Engine::kTransformId));

	DC_Engine::Rect ownerRect = pCreateBulletObject->GetOwnerRect();

	DC_Engine::Vector2_f norAngle = DC_Engine::Vector2_f::NormalizeAngle(ownerRect.angle);
	bulletTransform->Set_X(ownerRect.position.x + (float)(ownerRect.width / 2) + (norAngle.x * ownerRect.width));
	bulletTransform->Set_Y(ownerRect.position.y + (float)(ownerRect.height / 2) + (norAngle.y * ownerRect.height));

	bulletTransform->Set_Angle(ownerRect.angle);

	auto pBulletView = new BulletView(DC_Engine::ViewType::kBullet);
	pBulletView->SetGameObject(pGameOject);

	pBulletView->Init();
	AddView(std::unique_ptr<DC_Engine::DC_View>(pBulletView),pGameOject->GetId());
	m_pGameObjects[pGameOject->GetId()] = pGameOject;

}

void SandBox::DestroyObject(DC_Engine::Event* pEvent)
{
	auto pDestroyGameObject = static_cast<DestroyGameObjectEvent*>(pEvent);
	DestoryGameObject(pDestroyGameObject->GetId());
}

void SandBox::PlayMusic(DC_Engine::Event* pEvent)
{
	auto pMusic = static_cast<PlayMusicEvent*>(pEvent);
	DC_Engine::ApplicationLayer::GetInstance()->GetAudio().MusicPlay(m_pResouceFile->LoadResource(pMusic->GetFileName()));
}

void SandBox::PlaySound(DC_Engine::Event* pEvent)
{
	auto pSound = static_cast<PlaySoundEvent*>(pEvent);
	DC_Engine::ApplicationLayer::GetInstance()->GetAudio().SoundPlay(m_pResouceFile->LoadResource(pSound->GetFileName()));
}

// TODO player sound

void SandBox::ButtonPressed(DC_Engine::Event* pEvent)
{
	auto pButtonPress = static_cast<ButtonPressedEvent*>(pEvent);
	auto id = pButtonPress->GetEventIdToProcess();

	if (PlayMusicEvent::kEventType == id)
	{
		m_pEventManager->QueueEvent(std::make_unique<PlayMusicEvent>("Music/BackGroundMusic.mp3"));
	}
	else if (CreateGameObjectEvent::kEventType == id)
	{
		m_pEventManager->QueueEvent(std::make_unique<CreateGameObjectEvent>("GameObjects/Enemy.xml", DC_Engine::kEnemyViewId));
	}
	else if (EndGameEvent::kEventType == id)
	{

		DC_Engine::ApplicationLayer::GetInstance()->Exit();
	}

}

void SandBox::EndGame(DC_Engine::Event*)
{
	DC_Engine::ApplicationLayer::GetInstance()->Exit();
}

void SandBox::DestoryGameObject(uint32_t id)
{
	m_pGameObjects.erase(id);
	m_pViews.erase(id);
}


void SandBox::AddView(std::unique_ptr<DC_Engine::DC_View> pView, ViewId viewId)
{
	m_pViews[viewId] = std::move(pView);
}

std::unique_ptr<DC_Engine::DC_ObjectComponent> SandBox::CreateTransformComponent(DC_Engine::GameObject* pOwner, std::string name)
{
	return std::make_unique<TransformComponent>(pOwner, name);
}

std::unique_ptr<DC_Engine::DC_ObjectComponent> SandBox::CreateImageRenderComponent(DC_Engine::GameObject* pOwner, std::string name)
{
	return std::make_unique<ImageRenderComponent>(pOwner,name);
}

std::unique_ptr<DC_Engine::DC_ObjectComponent> SandBox::CreateSoundComponent(DC_Engine::GameObject* pOwner, std::string name)
{
	return std::make_unique<SoundComponent>(pOwner,name);
}

std::unique_ptr<DC_Engine::DC_ObjectComponent> SandBox::CreateTriggerBoxComponent(DC_Engine::GameObject* pOwner, std::string name)
{
	return std::make_unique<TriggerBoxComponent>(pOwner, name);
}

std::unique_ptr<DC_Engine::DC_ObjectComponent> SandBox::CreateEventActionComponent(DC_Engine::GameObject* pOwner, std::string name)
{
	return std::make_unique<EventActionComponent>(pOwner, name);
}

 
void SandBox::Exit()
{
	for (auto& view : m_pViews)
		view.second->Destroy();
}