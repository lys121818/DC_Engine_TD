#pragma once
// Include from DontCrashEngine
#include <DC_ObjectComponent.h>
#include <DC_Rect.h>

// Include from STL
#include <unordered_map>

// Include from DontCrashGame
#include "TransformComponent.h"

class ImageRenderComponent : public DC_Engine::DC_ObjectComponent
{
	// Functions
public:
	ImageRenderComponent(DC_Engine::GameObject* pOwner, const std::string& name);
	~ImageRenderComponent();
	// Inherited via DC_ObjectComponent
	bool Init(tinyxml2::XMLElement* pData) override;
	bool PostInit() override;
	void Render(DC_Engine::EngineGraphics* pRenderer) override;

	void SetState(const std::string& state) { m_currentState = state; }

	std::string GetState() { return m_currentState; }
	// Variable
public:
private:
	std::string m_currentState;
	std::string m_imageSource;
	std::unordered_map<std::string, DC_Engine::Rect> m_textureFrame;
	TransformComponent* m_pTextureTransform;
};

