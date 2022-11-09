#include "Input.h"

Input* Input::_instance = nullptr;

void Input::Init(void)
{
	if (_instance == nullptr)
		_instance = new Input();
}

void Input::Release(void)
{
	if (_instance != nullptr)
		delete _instance;
}

Input& Input::GetInstance(void)
{
	return *_instance;
}

void Input::SetKeyState(int vk, bool flag)
{
	keyState[vk] = flag;
}

bool Input::GetKeyState(int vk)
{
	if (keyState.find(vk) != keyState.end())
		return keyState[vk];

	return false; 
}

Input::Input()
	:keyState()
{
}

Input::~Input()
{
}
