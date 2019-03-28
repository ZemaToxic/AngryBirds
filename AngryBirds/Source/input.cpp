#include "../Header/input.h"
Input* Input::s_pInput = nullptr;

Input::Input() { m_CurrentState = INPUT_RELEASE; }

Input::~Input() { m_CurrentState = INPUT_RELEASE; }
