#include "Core.h"
#include "GameManager/GameManager.h"
#include <time.h>
Core* Core::m_pInst = NULL;


Core::Core()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(155);
}

Core::~Core()
{

}

bool Core::Init(HINSTANCE hInst)
{
	this->m_hInst = hInst;

	m_tWndSize.cx = 1280;
	m_tWndSize.cy = 720;

	WNDCLASSEX WndClass;

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.lpfnWndProc = WndProc;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = m_hInst;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"Class Name";
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	RECT rt = { 0,0,1280,720 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, NULL);
	m_hWnd = CreateWindow(L"Class Name", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		rt.right - rt.left, rt.bottom - rt.top, NULL, NULL, m_hInst, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}


int Core::Run()
{
	MSG Message;

	//while (GetMessage(&Message, NULL, 0, 0)) {
	//	TranslateMessage(&Message);
	//	DispatchMessage(&Message);

	//	if (!getGameLoop()) break;
	//}

	HDC hdc = NULL;

	QueryPerformanceFrequency(&m_Sec);
	QueryPerformanceCounter(&m_Time);

	GameManager::GetInst()->init();

	while (getGameLoop()) {
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			// todo
			LARGE_INTEGER tTime;
			QueryPerformanceCounter(&tTime);

			m_fDeltaTime = (tTime.QuadPart - m_Time.QuadPart) / (float)m_Sec.QuadPart;

			m_fTimeCnt += m_fDeltaTime;

			m_Time = tTime;

			GameManager::GetInst()->input(m_fDeltaTime);
			GameManager::GetInst()->update(m_fDeltaTime);
			GameManager::GetInst()->collision();
			GameManager::GetInst()->render(hdc);

#ifdef DEBUG
			TCHAR szTitle[30];
			swprintf(szTitle, L"FPS : %.1f", 1 / m_fDeltaTime);
			SetConsoleTitle(szTitle);
#endif // DEBUG


		}
	}

	GameManager::DestroyInst();

	return Message.wParam;
}


LRESULT Core::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = NULL, memdc;
	PAINTSTRUCT ps;

	HBITMAP hBitmap;

	switch (Msg) {
	case WM_CREATE:
		//GameManager::GetInst()->init();

		// Set Main Timer;
		//SetTimer(hWnd, 0, 1, NULL);
		break;

	case WM_PAINT:
		break;
		
	case WM_KEYDOWN:
		if (wParam == 'g') Core::GetInst()->setGridShow();
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		Core::GetInst()->setGameLoopFalse();
		//GameManager::DestroyInst();
		break;

	case WM_TIMER:
		switch (wParam) {
		case 0:
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
