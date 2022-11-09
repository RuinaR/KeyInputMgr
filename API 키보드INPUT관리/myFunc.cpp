#include "myFunc.h"
#include "Input.h"
#include <time.h>

//윈도우 관련
HINSTANCE g_hInst;
RECT windowRect;
int screenWidth;
int screenHeight;
//HWND
HWND hWndMain = NULL;

void Init(HINSTANCE hInstance, int nCmdShow, const RECT* winRect)
{
	srand((unsigned int)time(NULL));

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	windowRect = *winRect;
	hWndMain = MakeWindow(hInstance, winRect, WndProcMain, TEXT("메인"), WHITE_BRUSH, nCmdShow);
}


double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my, int r)
{
	if (LengthPts(x, y, mx, my) < r)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL InRect(const RECT* rect, int mx, int my)
{
	if (rect->left < mx &&
		rect->right > mx &&
		rect->top < my &&
		rect->bottom > my)
	{
		return TRUE;
	}
	return FALSE;
}

void VectorNormalize(POS* vector)
{
	double d = sqrt((vector->x * vector->x) + (vector->y * vector->y));
	vector->x = vector->x / d;
	vector->y = vector->y / d;
}

BOOL SetNoIntersect(const LPRECT pHold, LPRECT pRect)
{
	RECT rcInter = { 0 };
	if (IntersectRect(&rcInter, pHold, pRect))
	{
		int nW = rcInter.right - rcInter.left;
		int nH = rcInter.bottom - rcInter.top;

		if (nW > nH)
		{
			if (rcInter.top == pHold->top)
			{
				pRect->top -= nH;
				pRect->bottom -= nH;
			}
			else if (rcInter.bottom == pHold->bottom)
			{
				pRect->top += nH;
				pRect->bottom += nH;
			}
		}
		else
		{
			if (rcInter.left == pHold->left)
			{
				pRect->left -= nW;
				pRect->right -= nW;
			}
			else if (rcInter.right == pHold->right)
			{
				pRect->left += nW;
				pRect->right += nW;
			}
		}
		return TRUE;
	}
	return FALSE;
}


LRESULT CALLBACK WndProcMain(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static PAINTSTRUCT ps;

	const static int timer = 1;
	//더블버퍼링용 변수
	static HDC hdcBuff;
	static HBITMAP hBmpBuff;
	static HBITMAP hBmpBuffOld;
	static RECT clientRect = { 0 };
	//
	static HBRUSH brush;

	switch (iMessage)
	{
	case WM_CREATE:
	{
		GetClientRect(hWnd, &clientRect);
		SetTimer(hWnd, timer, TIMERCNT, NULL);
		break;
	}
	case WM_GETMINMAXINFO:
	{
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = windowRect.right - windowRect.left;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = windowRect.bottom - windowRect.top;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = windowRect.right - windowRect.left;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_TIMER:
	{
		if (wParam == timer)
		{

			InvalidateRect(hWnd, NULL, false);
		}
		break;
	}
	case WM_KEYDOWN:
		Input::GetInstance().SetKeyState(wParam, true);
		break;
	case WM_KEYUP:
		Input::GetInstance().SetKeyState(wParam, false);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		//더블 버퍼링용 DC, BMP
		hdcBuff = CreateCompatibleDC(hdc);
		hBmpBuff = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		hBmpBuffOld = (HBITMAP)SelectObject(hdcBuff, hBmpBuff);
		PatBlt(hdcBuff, 0, 0, clientRect.right, clientRect.bottom, WHITENESS);

		//버퍼에 그리기

		//Input Test////////////////////////////////////////////////////
		int x = 10;
		int xUnit = 60;
		int y = 100;
		int yUnit = 30;
		WCHAR tmp[30];
		for (map<int, bool>::iterator iter = Input::GetInstance().GetKeyMap().begin(); iter != Input::GetInstance().GetKeyMap().end(); iter++)
		{
			wsprintf(tmp, L"[%x , %d] ", (*iter).first, (*iter).second);
			TextOut(hdcBuff, x, y, tmp, lstrlen(tmp));
			x += xUnit;
			if (x >= 160)
			{
				x = 10;
				y += yUnit;
			}
		}
		////////////////////////////////////////////////////////////////

		//더블 버퍼링 쓰기
		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuff, 0, 0, SRCCOPY);
		SelectObject(hdcBuff, hBmpBuffOld);
		//delete
		DeleteObject(hBmpBuff);
		DeleteDC(hdcBuff);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		KillTimer(hWnd, timer);
		PostQuitMessage(0);
		break;
	}
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


HWND MakeWindow(HINSTANCE hInstance, const RECT* rect, WNDPROC winproc, LPCTSTR name, int BackColor, int nCmdShow)
{										//윈도우 핸들 선언
	MSG Message;										//메세지 구조체 변수 선언
	WNDCLASS WndClass;									//Windows Class 구조체 변수 선언
	g_hInst = hInstance;								//hInstance값을 외부에서도 사용할 수 있도록 전역변수에 값을 저장

	WndClass.cbClsExtra = 0;							//예약 영역. 지금은 사용X
	WndClass.cbWndExtra = 0;							//예약 영역
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BackColor);	// 윈도우의 배경 색상을 지정
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//윈도우의 마우스포인터 모양을 지정
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//윈도우의 커서 아이콘 모양 지정
	WndClass.hInstance = hInstance;						//윈도우 클래스를 등록하는 프로그램 번호
	WndClass.lpfnWndProc = winproc;						//윈도우 메세지 처리 함수 지정
	WndClass.lpszClassName = name;					//윈도우 클래스의 이름 지정
	WndClass.lpszMenuName = NULL;						//메뉴 지정
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//윈도우의 스타일을 정의

	RegisterClass(&WndClass);							//WNDCLASS 구조체의 번지를 전달

	HWND hWnd = CreateWindow(name, name,			//윈도우를 생성
		WS_OVERLAPPEDWINDOW,
		rect->left, rect->top, rect->right, rect->bottom, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	return hWnd;
}


