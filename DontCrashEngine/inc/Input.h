#pragma once
#include <memory>
#include <array>
#include <vector>

namespace DC_Engine
{
	class KeyboardInputEvent
	{
	public:
		enum KeyCode
		{
			// Alphabet key codes
			kCode_A, kCode_B, kCode_C, kCode_D, kCode_E,
			kCode_F, kCode_G, kCode_H, kCode_I, kCode_J,
			kCode_K, kCode_L, kCode_M, kCode_N, kCode_O,
			kCode_P, kCode_Q, kCode_R, kCode_S, kCode_T,
			kCode_U, kCode_V, kCode_W, kCode_X, kCode_Y,
			kCode_Z,

			// Number key codes
			kCode_1, kCode_2, kCode_3, kCode_4, kCode_5,
			kCode_6, kCode_7, kCode_8, kCode_9, kCode_0,

			kCode_Escape,
			kCode_Enter,
			kCode_Spacebar,
			kCode_LeftControl,
			kCode_RightControl,
			kCode_LeftAlt,
			kCode_RightAlt,
			kCode_LeftShift,
			kCode_RightShift,

			kCode_ArrowUp,
			kCode_ArrowDown,
			kCode_ArrowLeft,
			kCode_ArrowRight,

			kCode_Max
		};

		virtual ~KeyboardInputEvent() {}
		virtual bool Init();
		virtual void SetKeyState(KeyCode key, bool down);
		inline virtual bool IsKeyDown(KeyCode key) { return m_keyState[key]; }
		inline virtual bool IsKeyPressed(KeyCode key) { return m_keyState[key] && !m_prevKeyState[key]; }
		inline virtual bool IsKeyReleased(KeyCode key) { return !m_keyState[key] && m_prevKeyState[key]; }
		inline virtual void NextFrame() { m_prevKeyState = m_keyState; }

		static std::unique_ptr<KeyboardInputEvent> Create();
	protected:
		std::array<bool, kCode_Max> m_keyState;
		std::array<bool, kCode_Max> m_prevKeyState;

	};

	class MouseInput
	{
	public:
		enum Button
		{
			kButtonLeft,
			kButtonRight,

			kButtonMax
		};

		virtual ~MouseInput() {}
		virtual bool Init();
		virtual void SetButtonState(Button button, bool down);
		inline virtual bool IsButtonDown(Button button) { return m_buttonState[button]; }
		inline virtual bool IsButtonPressed(Button button) { return m_buttonState[button] && !m_prevButtonState[button]; }
		inline virtual bool IsButtonReleased(Button button) { return !m_buttonState[button] && m_prevButtonState[button]; }
		inline virtual void NextFrame() { m_prevButtonState = m_buttonState; }

		static std::unique_ptr<MouseInput> Create();
	protected:
		std::array<bool, kButtonMax> m_buttonState;
		std::array<bool, kButtonMax> m_prevButtonState;
	};

	class MouseMove
	{
	public:
		virtual bool Init();

		virtual void SetMousePosition(float x, float y);

		inline virtual std::pair<float, float> GetMousePosition() const { return { m_mouseX, m_mouseY }; }
		inline virtual std::pair<float, float> GetPreviousMousePosition() const { return { m_prevMouseX, m_prevMouseY }; }
		inline virtual std::pair<float, float> GetMouseDelta() const { return { m_deltaX, m_deltaY }; }
		virtual bool IsMouseMoved();

		// Prepare for the next frame, by updating previous position
		inline virtual void NextFrame();

		static std::unique_ptr<MouseMove> Create();

	protected:
		float m_mouseX, m_mouseY;        // Current mouse position
		float m_prevMouseX, m_prevMouseY; // Previous mouse position
		float m_deltaX, m_deltaY;         // Mouse delta (movement between frames)
	};
}