#include "myFunc.h"
#include "Input.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	RECT winRect = { 10,10,1610,1010 };

	Input::Init();
	Init(hInstance, nCmdShow, &winRect);

	while (GetMessage(&Message, NULL, 0, 0)) 
	{			
		TranslateMessage(&Message);																							
		DispatchMessage(&Message);								
	}

	Input::Release();
	return (int)Message.wParam;							
}

