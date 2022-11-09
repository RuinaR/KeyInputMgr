#include "myFunc.h"
#include "Input.h"
#include <time.h>

//������ ����
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
	hWndMain = MakeWindow(hInstance, winRect, WndProcMain, TEXT("����"), WHITE_BRUSH, nCmdShow);
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
	//������۸��� ����
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

		//���� ���۸��� DC, BMP
		hdcBuff = CreateCompatibleDC(hdc);
		hBmpBuff = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		hBmpBuffOld = (HBITMAP)SelectObject(hdcBuff, hBmpBuff);
		PatBlt(hdcBuff, 0, 0, clientRect.right, clientRect.bottom, WHITENESS);

		//���ۿ� �׸���

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

		//���� ���۸� ����
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
{										//������ �ڵ� ����
	MSG Message;										//�޼��� ����ü ���� ����
	WNDCLASS WndClass;									//Windows Class ����ü ���� ����
	g_hInst = hInstance;								//hInstance���� �ܺο����� ����� �� �ֵ��� ���������� ���� ����

	WndClass.cbClsExtra = 0;							//���� ����. ������ ���X
	WndClass.cbWndExtra = 0;							//���� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BackColor);	// �������� ��� ������ ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//�������� ���콺������ ����� ����
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//�������� Ŀ�� ������ ��� ����
	WndClass.hInstance = hInstance;						//������ Ŭ������ ����ϴ� ���α׷� ��ȣ
	WndClass.lpfnWndProc = winproc;						//������ �޼��� ó�� �Լ� ����
	WndClass.lpszClassName = name;					//������ Ŭ������ �̸� ����
	WndClass.lpszMenuName = NULL;						//�޴� ����
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//�������� ��Ÿ���� ����

	RegisterClass(&WndClass);							//WNDCLASS ����ü�� ������ ����

	HWND hWnd = CreateWindow(name, name,			//�����츦 ����
		WS_OVERLAPPEDWINDOW,
		rect->left, rect->top, rect->right, rect->bottom, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	return hWnd;
}


