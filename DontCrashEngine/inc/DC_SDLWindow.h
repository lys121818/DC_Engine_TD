#pragma once
#include "WindowManager.h"
#include "Input.h"
struct SDL_Window;


/*========================================================================

  DC_SDLMachine - SDL using API

========================================================================*/


namespace DC_Engine
{
	enum class ActionResult;

	// #DG: keep your naming convention the same between parent/child classes
	// here the base is "platform" but the child is "window"
	// this confuses the meaning of what this should really be doing
	class DC_SDLWindow : public WindowManager
	{
	public:
		DC_SDLWindow(const WindowConfig& config);
		~DC_SDLWindow() override;

		ActionResult Init() override;
		ActionResult Destroy() override;

		inline void AttachKeyboard(std::unique_ptr<KeyboardInputEvent> pInput) override { m_pKeyboard = std::move(pInput); }
		inline void AttachMouseButton(std::unique_ptr<MouseInput> pInput) override { m_pMouseInput = std::move(pInput); }
		inline void AttachMouseMove(std::unique_ptr<MouseMove> pInput) override { m_pMouseMove = std::move(pInput); }

		void NextFrame() override;

		std::unique_ptr<KeyboardInputEvent>& GetKeyboard() override;
		std::unique_ptr<MouseInput>& GetMouseInput() override;
		std::unique_ptr<MouseMove>& GetMouseMove() override;

		bool Update(float) override;

		void* GetWindow() const override { return m_pDCWindow;}

		std::vector<std::string> GetAllFiles(const std::string& path) override;
		std::vector<char> GetBinaryFromFile(const std::string& path) override;

	private:
		bool ProcessEvents();

		KeyboardInputEvent::KeyCode ConverKeyCode(const uint32_t& key) const;
		MouseInput::Button ConverButton(const uint32_t& button) const;

		void GetFilesRecursive(const std::string& path, const std::string& directory, std::vector<std::string>& files);

		std::vector<char> ReadBinary(const std::string& path);
		std::vector<char> ReadBinaryUsingRW(const std::string& path);

	private:
		SDL_Window* m_pDCWindow = nullptr;
	};


}
