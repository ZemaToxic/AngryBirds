#pragma once
#include "utils.h"

enum InputState //Put later in utils
{
	INPUT_FIRST_RELEASE,
	INPUT_RELEASE,
	INPUT_FIRST_PRESS,
	INPUT_HOLD
};

enum InputMouse //Put later in utils
{
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,
};

class Input
{
public:
	~Input();

	InputState GetKeyState(char key)
	{
		return (KeyState[key]);
	}

	//Keyboard states, these funcs go in main
	/*********************************************/
	void KeyboardUp(unsigned char key, int x, int y)
	{
		cLastKeyPressed = key;

		KeyState[key] = INPUT_RELEASE;
		m_CurrentState = INPUT_RELEASE;
	}

	void KeyboardDown(unsigned char key, int x, int y)
	{
		cLastKeyPressed = key;

		KeyState[key] = INPUT_HOLD;
		m_CurrentState = INPUT_HOLD;
	}

	//Mouse states, these go in main too
	/*********************************************/
	void MouseClick(int button, int glutState, int x, int y)
	{
		if (button < 3)
		{
			m_MousePos = glm::vec3(x, y, 0.0f);
			MouseState[button] = (glutState == GLUT_DOWN) ? INPUT_HOLD : INPUT_RELEASE;
		}
	}

	void MouseMove(int x, int y) { m_MousePos = glm::vec3(x, y, 0.0f); };

	glm::vec3 GetMousePos()
	{
		return (m_MousePos);
	}

	InputState GetMouseState(int button)
	{
		if (MouseState[button] == INPUT_HOLD && button < 3)
		{
			return (INPUT_HOLD);
		}
		return (INPUT_RELEASE);
	}

	//Singleton setup
	/*********************************************/

	static Input& GetInstance()
	{
		if (s_pInput == nullptr)
		{
			s_pInput = new Input();
		}

		return (*s_pInput);
	}

	static void DestroyInstance()
	{
		delete s_pInput;
		s_pInput = nullptr;
	}

protected:
	static Input* s_pInput;

private:
	Input();
	Input(const Input& _kr) = delete;
	Input& operator=(const Input& _kr) = delete;

	InputState KeyState[255];
	InputState MouseState[3];

	glm::vec3 m_MousePos;

	char cLastKeyPressed;
	InputState m_CurrentState;
};
