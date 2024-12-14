#include <SDL.h>
#include <SDL_syswm.h>
#include <string>
#include <vector>
#include <fstream>
#include <direct.h>
#include "Input.h"
#include "DC_SDLWindow.h"
#include "ActionResult.h"
#include "ApplicationLayer.h"

namespace DC_Engine
{

	DC_SDLWindow::DC_SDLWindow(const WindowConfig& config)
	{
		m_config = config;

		m_config.title += ".SDL";
	}

	DC_SDLWindow::~DC_SDLWindow()
	{
		// Destructor
	}

	ActionResult DC_SDLWindow::Init()
	{
		ActionResult result = ActionResult::kSuccess;

		// Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::string errorText = "failed to initialize SDL. Error: ";
			errorText += SDL_GetError();
			ApplicationLayer::GetInstance()->GetLog().LogError(errorText);

			return ActionResult::kWindowInitFail;
		}

		// Create windows
		m_pDCWindow = SDL_CreateWindow(m_config.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)m_config.width, (int)m_config.height, SDL_WINDOW_RESIZABLE);

		if (m_pDCWindow == nullptr)
		{
			std::string errorText = "failed to Create Window. Erorr:  ";
			errorText += SDL_GetError();
			ApplicationLayer::GetInstance()->GetLog().LogError(errorText);

			return ActionResult::kWindowCreateFail;
		}


		return result;
	}

	ActionResult DC_SDLWindow::Destroy()
	{
		if (m_pDCWindow)
		{
			SDL_DestroyWindow(m_pDCWindow);
			SDL_Quit();
			m_pDCWindow = nullptr;
		}

		return ActionResult::kSuccess;
	}



	void DC_SDLWindow::NextFrame()
	{
		if (m_pKeyboard)
			m_pKeyboard->NextFrame();
		if (m_pMouseInput)
			m_pMouseInput->NextFrame();
		if (m_pMouseMove)
			m_pMouseMove->NextFrame();
	}

	std::unique_ptr<KeyboardInputEvent>& DC_SDLWindow::GetKeyboard()
	{
		return m_pKeyboard;
	}

	std::unique_ptr<MouseInput>& DC_SDLWindow::GetMouseInput()
	{
		return m_pMouseInput;
	}

	std::unique_ptr<MouseMove>& DC_SDLWindow::GetMouseMove()
	{
		return m_pMouseMove;
	}

	bool DC_SDLWindow::Update(float)
	{

		return ProcessEvents();
	}

	std::vector<std::string> DC_SDLWindow::GetAllFiles(const std::string& path)
	{
		std::vector<std::string> files;
		GetFilesRecursive(path, "", files);

		return files;
	}

	std::vector<char> DC_SDLWindow::GetBinaryFromFile(const std::string& path)
	{
		const char* fileType = std::strrchr(path.c_str(), '.');

		if (fileType)
		{
			fileType++;

			if (std::strcmp(fileType, "png") == 0 || std::strcmp(fileType, "jpg") == 0 || std::strcmp(fileType, "tiff") == 0 || std::strcmp(fileType, "bmp") == 0)
			{
				return std::move(ReadBinaryUsingRW(path));
			}
			else if (std::strcmp(fileType, "mp3") == 0 || std::strcmp(fileType, "ogg") == 0 || std::strcmp(fileType, "wav") == 0)
			{
				return std::move(ReadBinaryUsingRW(path));
			}
			else
			{
				return std::move(ReadBinary(path));
			}
		}
		else
		{
			return std::vector<char>();
		}

	}

	bool DC_SDLWindow::ProcessEvents()
	{
		
		SDL_Event evt;
		
		while (SDL_PollEvent(&evt) != 0)
		{
			switch (evt.type)
			{
				// Window Event
				case SDL_WINDOWEVENT:
				{
					switch (evt.window.event)
					{
					case SDL_WINDOWEVENT_CLOSE:
					{
						return false;
					}

					default:
						break; // switch(evt.window.event)
					}


					break; // SDL_WINDOWEVENT
				}
				// KeyboardEvent
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					if (m_pKeyboard)
					{
						KeyboardInputEvent::KeyCode code = ConverKeyCode(evt.key.keysym.scancode);
						m_pKeyboard->SetKeyState(code, evt.type == SDL_KEYDOWN);
					}
					break;
				}
				
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				{
					if (m_pMouseInput)
					{
						MouseInput::Button button = ConverButton(evt.button.button);
						m_pMouseInput->SetButtonState(button, evt.type == SDL_MOUSEBUTTONDOWN);
					}
					break;
				}
				case SDL_MOUSEMOTION:
				{
					if (m_pMouseMove)
						m_pMouseMove->SetMousePosition((float)evt.motion.x, (float)evt.motion.y);
					break;
				}
				
			default:
				break; // switch (evt.type)
			}
		}

		return true;

	}
	KeyboardInputEvent::KeyCode DC_SDLWindow::ConverKeyCode(const uint32_t& key) const
	{
		KeyboardInputEvent::KeyCode convertedKey = KeyboardInputEvent::KeyCode::kCode_Max;

		// Conver Alphabets
		if (key >= SDL_SCANCODE_A && key <= SDL_SCANCODE_Z)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(key - SDL_SCANCODE_A + KeyboardInputEvent::KeyCode::kCode_A);
		}
		// Conver Numbers
		else if(key == SDL_SCANCODE_0)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_0);
		}
		else if (key >= SDL_SCANCODE_1 && key <= SDL_SCANCODE_9)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(key - SDL_SCANCODE_1 + KeyboardInputEvent::KeyCode::kCode_1);
		}
		else if (key == SDL_SCANCODE_ESCAPE)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_Escape);
		}
		else if (key == SDL_SCANCODE_RETURN)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_Enter);
		}
		else if (key == SDL_SCANCODE_SPACE)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_Spacebar);
		}
		else if (key == SDL_SCANCODE_LCTRL)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_LeftControl);
		}
		else if (key == SDL_SCANCODE_RCTRL)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_RightControl);
		}
		else if (key == SDL_SCANCODE_LALT)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_LeftAlt);
		}
		else if (key == SDL_SCANCODE_RALT)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_RightAlt);
		}
		else if (key == SDL_SCANCODE_LSHIFT)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_LeftShift);
		}
		else if (key == SDL_SCANCODE_RSHIFT)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_RightShift);
		}
		else if (key == SDL_SCANCODE_UP)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_ArrowUp);
		}
		else if (key == SDL_SCANCODE_DOWN)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_ArrowDown);
		}
		else if (key == SDL_SCANCODE_LEFT)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_ArrowLeft);
		}
		else if (key == SDL_SCANCODE_RIGHT)
		{
			convertedKey = static_cast<KeyboardInputEvent::KeyCode>(KeyboardInputEvent::KeyCode::kCode_ArrowRight);
		}

		return convertedKey;
	}
	MouseInput::Button DC_SDLWindow::ConverButton(const uint32_t& button) const
	{

		static const uint32_t kSDLLeftButton = 1;
		static const uint32_t kSDLRightButton = 3;

		if (button == kSDLLeftButton)
		{
			return MouseInput::kButtonLeft;
		}
		else if (button == kSDLRightButton)
		{
			return MouseInput::kButtonRight;
		}
		
		return MouseInput::Button::kButtonMax;
			
	}
	void DC_SDLWindow::GetFilesRecursive(const std::string& path, const std::string& directory, std::vector<std::string>& files)
	{
		std::string fullPath = path + '/' + directory + "/*";
		WIN32_FIND_DATAA findData;
		HANDLE findHandle = FindFirstFileA(fullPath.c_str(), &findData);
		if (findHandle == INVALID_HANDLE_VALUE)
			return;

		std::string rootDirectory = directory;
		if (!rootDirectory.empty())
			rootDirectory += '/';

		while (true)
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::string directoryName = findData.cFileName;
				if (directoryName != "." && directoryName != "..")
				{
					std::string nextDirectory = rootDirectory + directoryName;
					GetFilesRecursive(path, nextDirectory, files);
				}
			}
			else
				files.emplace_back(rootDirectory + findData.cFileName);

			bool result = FindNextFileA(findHandle, &findData);
			if (!result)
				break;
		}
	}

	std::vector<char> DC_SDLWindow::ReadBinary(const std::string& path)
	{
		std::vector<char> data;

		std::fstream resourceFile(path, std::ios_base::in, std::ios_base::binary);
		if (resourceFile.is_open())
		{
			resourceFile.seekg(0, resourceFile.end);
			size_t fileSize = static_cast<size_t>(resourceFile.tellg());
			resourceFile.seekg(0, resourceFile.beg);

			data.resize(fileSize);
			resourceFile.read(data.data(), fileSize);
		}

		return data;
	}
	std::vector<char> DC_SDLWindow::ReadBinaryUsingRW(const std::string& path)
	{
		std::vector<char> data;

		// Read binary from the file
		SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");

		if (!rw)
		{
			return data;
		}

		// Seek through and get size of the file
		Sint64 size = SDL_RWsize(rw);
		if (size < 0)
		{
			SDL_RWclose(rw);
			return data;
		}

		data.resize(static_cast<Sint64>(size));

		// error check
		Sint64 bytesRead = SDL_RWread(rw, data.data(), 1, size);
		if (bytesRead != static_cast<Sint64>(size)) {
			data.clear(); // Clear data if the read was incomplete
		}

		SDL_RWclose(rw);

		return data;
	}
}
