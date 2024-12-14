#include "DC_OpenGLWindow.h"
#include <glfw3.h>
#include "ActionResult.h"
#include "ApplicationLayer.h"

// access to the native windows handle
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>


namespace DC_Engine
{
	DC_OpenGLWindow::DC_OpenGLWindow(const WindowConfig& config)
	{
		m_config = config;

		m_config.title += ".OpenGL";
	}

	DC_OpenGLWindow::~DC_OpenGLWindow()
	{
		// Destructor
	}

	ActionResult DC_OpenGLWindow::Init()
	{
		ActionResult result = ActionResult::kSuccess;

		// Initialize OpenGL
		if (glfwInit() == 0)
		{
			std::string errorText = "failed to initialize OpenGL. Error: ";
			errorText += std::to_string(glGetError());
			ApplicationLayer::GetInstance()->GetLog().LogError(errorText);
		}

		// Create windows
		m_pDCWindow = glfwCreateWindow((int)m_config.width, (int)m_config.height, m_config.title.c_str(), NULL, NULL);

		if (m_pDCWindow == nullptr)
		{
			std::string errorText = "failed to Create Window. Erorr: ";
			errorText += std::to_string(glGetError());
			ApplicationLayer::GetInstance()->GetLog().LogError(errorText);

			Destroy();
			return ActionResult::kWindowCreateFail;
		}

		return result;
	}

	ActionResult DC_OpenGLWindow::Destroy()
	{
		if (m_pDCWindow)
		{
			glfwTerminate();
			m_pDCWindow = nullptr;
		}


		return ActionResult::kSuccess;
	}

	void DC_OpenGLWindow::NextFrame()
	{
	}

	std::unique_ptr<KeyboardInputEvent>& DC_OpenGLWindow::GetKeyboard()
	{
		return m_pKeyboard;
	}

	std::unique_ptr<MouseInput>& DC_OpenGLWindow::GetMouseInput()
	{
		return m_pMouseInput;
	}

	std::unique_ptr<MouseMove>& DC_OpenGLWindow::GetMouseMove()
	{
		return m_pMouseMove;
	}

	bool DC_OpenGLWindow::Update(float)
	{
		bool isRunning;

		isRunning = !glfwWindowShouldClose(m_pDCWindow);

		if (isRunning)
		{
			//Render
			glClear(GL_COLOR_BUFFER_BIT);

			// Event handler
			ProcessEvents();

			// Swap front and back buffers
			glfwSwapBuffers(m_pDCWindow);
		}
		/*
		*/

		return isRunning;
	}

	std::vector<std::string> DC_OpenGLWindow::GetAllFiles(const std::string&)
	{
		return std::vector<std::string>();
	}

	std::vector<char> DC_OpenGLWindow::GetBinaryFromFile(const std::string&)
	{
		return std::vector<char>();
	}


	bool DC_OpenGLWindow::ProcessEvents()
	{
		glfwPollEvents();

		return true;
	}

}