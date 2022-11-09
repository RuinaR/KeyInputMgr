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

bool Input::SetKeyState(int vk, bool flag)
{
	if (keyState.find(vk) != keyState.end())
	{
		keyState[vk] = flag;
		return true;
	}
	return false;
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
	keyState[VK_LEFT] = false;
	keyState[VK_RIGHT] = false;
	keyState[VK_UP] = false;
	keyState[VK_DOWN] = false;

	keyState[VK_SPACE] = false;
	keyState[VK_RETURN] = false;
	keyState[VK_ESCAPE] = false;

	keyState[VK_SHIFT] = false;
	keyState[VK_CONTROL] = false;
	keyState[VK_MENU] = false;

	keyState[VK_0] = false;
	keyState[VK_1] = false;
	keyState[VK_2] = false;
	keyState[VK_3] = false;
	keyState[VK_4] = false;
	keyState[VK_5] = false;
	keyState[VK_6] = false;
	keyState[VK_7] = false;
	keyState[VK_8] = false;
	keyState[VK_9] = false;

	keyState[VK_A] = false;
	keyState[VK_B] = false;
	keyState[VK_C] = false;
	keyState[VK_D] = false;
	keyState[VK_E] = false;
	keyState[VK_F] = false;
	keyState[VK_G] = false;
	keyState[VK_H] = false;
	keyState[VK_I] = false;
	keyState[VK_J] = false;
	keyState[VK_K] = false;
	keyState[VK_L] = false;
	keyState[VK_M] = false;
	keyState[VK_N] = false;
	keyState[VK_O] = false;
	keyState[VK_P] = false;
	keyState[VK_Q] = false;
	keyState[VK_R] = false;
	keyState[VK_S] = false;
	keyState[VK_T] = false;
	keyState[VK_U] = false;
	keyState[VK_V] = false;
	keyState[VK_W] = false;
	keyState[VK_X] = false;
	keyState[VK_Y] = false;
	keyState[VK_Z] = false;
}

Input::~Input()
{
}
