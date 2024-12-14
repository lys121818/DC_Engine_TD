#pragma once
#include "WindowManager.h"

struct GLFWwindow;

/*========================================================================

  DC_SDLMachine - OpenGL using API

========================================================================*/

namespace DC_Engine
{
	class DC_OpenGLWindow : public WindowManager
	{
	public:
		DC_OpenGLWindow(const WindowConfig& config);
		virtual ~DC_OpenGLWindow() override;

		virtual ActionResult Init() override;
		virtual ActionResult Destroy() override;

		inline void AttachKeyboard(std::unique_ptr<KeyboardInputEvent> pInput) override { m_pKeyboard = std::move(pInput); }
		inline void AttachMouseButton(std::unique_ptr<MouseInput> pInput) override { m_pMouseInput = std::move(pInput); }
		inline void AttachMouseMove(std::unique_ptr<MouseMove> pInput) override { m_pMouseMove = std::move(pInput); }

		void NextFrame() override;

		std::unique_ptr<KeyboardInputEvent>& GetKeyboard() override;
		std::unique_ptr<MouseInput>& GetMouseInput() override;
		std::unique_ptr<MouseMove>& GetMouseMove() override;

		virtual bool Update(float) override;
		virtual void* GetWindow() const override { return m_pDCWindow; }

		// Todo: 
		std::vector<std::string> GetAllFiles(const std::string& path) override;
		std::vector<char> GetBinaryFromFile(const std::string& path) override;

	private:
		bool ProcessEvents();

	private:
		GLFWwindow* m_pDCWindow = nullptr;

	};

}