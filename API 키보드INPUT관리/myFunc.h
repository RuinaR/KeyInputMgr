#pragma once
#pragma once
#include <windows.h>		// 윈도우 헤더파일
#include <math.h>
#include <string.h>		
#include <list>
using namespace std;

#define TIMERCNT 10

typedef struct _POS
{
	double x;
	double y;
}POS;

void Init(HINSTANCE hInstance, int nCmdShow, const RECT* winRect);
double LengthPts(int x1, int y1, int x2, int y2);
BOOL InCircle(int x, int y, int mx, int my, int r);
BOOL InRect(const RECT* rect, int mx, int my);
void VectorNormalize(POS* vector);
BOOL SetNoIntersect(const LPRECT pHold, LPRECT pRect);
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
HWND MakeWindow(HINSTANCE hInstance, const RECT* rect, WNDPROC winproc, LPCTSTR name, int BackColor, int nCmdShow);
