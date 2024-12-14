#include "Input.h"

namespace DC_Engine
{
    // Keyboard
    std::unique_ptr<KeyboardInputEvent> KeyboardInputEvent::Create()
    {
        return std::make_unique<KeyboardInputEvent>();
    }
    bool KeyboardInputEvent::Init()
    {
        m_keyState = { false }; 
        return true; 
        
    }
    void KeyboardInputEvent::SetKeyState(KeyCode key, bool down)
    {
        if (key != kCode_Max) 
            m_keyState[key] = down;
    }

    // Mouse Button
    std::unique_ptr<MouseInput> MouseInput::Create()
    {
        return std::make_unique<MouseInput>();
    }
    bool MouseInput::Init()
    {
        m_buttonState = { false };

        return true;
    }
    void MouseInput::SetButtonState(Button button, bool down)
    {
        if (button != kButtonMax)
            m_buttonState[button] = down;
    }

    // Mouse Movement
    bool MouseMove::Init()
    {
        m_mouseX = 0.0;
        m_mouseY = 0.0;
        m_prevMouseX = 0.0;
        m_prevMouseY = 0.0;
        m_deltaX = 0.0;
        m_deltaY = 0.0;
        return true;
    }
    void MouseMove::SetMousePosition(float x, float y)
    {
        m_prevMouseX = m_mouseX;
        m_prevMouseY = m_mouseY;

        m_mouseX = x;
        m_mouseY = y;

        m_deltaX = m_mouseX - m_prevMouseX;
        m_deltaY = m_mouseY - m_prevMouseY;
    }
    bool MouseMove::IsMouseMoved()
    {
        if (m_deltaX != 0 || m_deltaY != 0)
            return true;

        return false;
    }
    inline void MouseMove::NextFrame()
    {
        m_prevMouseX = m_mouseX;
        m_prevMouseY = m_mouseY;
        m_deltaX = 0;
        m_deltaY = 0;
    }
    std::unique_ptr<MouseMove> MouseMove::Create()
    {
        return std::make_unique<MouseMove>();
    }
}
