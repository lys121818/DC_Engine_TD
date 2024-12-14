#pragma once
#include <string>
#include <memory>
#include <functional>
#include <vector>

#include "WindowConfig.h"
#include "Input.h"
/*==================================================================================

  EnigneAPI - This class controls the window using API

  ----------------------------------------------------------------------------------
  ActionResult: It returns the enum to specify the action was successful
  WindowConfig: A struct of window configuration

====================================================================================*/

namespace DC_Engine
{
	enum class ActionResult;

	class WindowManager
	{
	public:
		virtual ~WindowManager() {};

		virtual ActionResult Init() = 0;

		virtual bool Update(float) = 0;

		virtual ActionResult Destroy() = 0;

		virtual void NextFrame() = 0;

		virtual void AttachKeyboard(std::unique_ptr<KeyboardInputEvent> pInput) = 0;
		virtual void AttachMouseButton(std::unique_ptr<MouseInput> pInput) = 0;
		virtual void AttachMouseMove(std::unique_ptr<MouseMove> pInput) = 0;

		virtual std::unique_ptr<KeyboardInputEvent>& GetKeyboard() = 0;
		virtual std::unique_ptr<MouseInput>& GetMouseInput() = 0;
		virtual std::unique_ptr<MouseMove>& GetMouseMove() = 0;


		virtual const WindowConfig& GetWindowInfo() const { return m_config; }

		virtual void* GetWindow() const = 0;

		static std::unique_ptr<WindowManager> Create(const WindowConfig& config);

		virtual std::vector<std::string> GetAllFiles(const std::string& path) = 0;
		virtual std::vector<char> GetBinaryFromFile(const std::string& path) = 0;

	protected:
		WindowConfig m_config;
		std::unique_ptr<KeyboardInputEvent> m_pKeyboard;
		std::unique_ptr<MouseInput> m_pMouseInput;
		std::unique_ptr<MouseMove> m_pMouseMove;
	};
}