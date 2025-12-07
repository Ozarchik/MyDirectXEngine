#include "Input.h"

Input::Input()
{
	memset(m_keys, 0, sizeof(m_keys));
}

void Input::KeyUp(unsigned int key)
{
	m_keys[key] = true;
}

void Input::KeyDown(unsigned int key)
{
	m_keys[key] = false;
}

bool Input::IsKeyUp(unsigned int key)
{
	return m_keys[key];
}

bool Input::IsKeyDown(unsigned int key)
{
	return !m_keys[key];
}
