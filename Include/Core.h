#pragma once

#include "Game.h"

class Core
{
private:
	static Core* m_pInst;

public:
	static Core* GetInst() {
		if (!m_pInst) m_pInst = new Core;
		return m_pInst;
	}

	static void DestroyInst() {
		SAFE_DELETE(m_pInst);
	}

private:
	Core();
	~Core();

private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hdc;
	SIZE		m_tWndSize;

	bool m_bShowGrid = false;

public:
	SIZE GetSize() const { return m_tWndSize; }
	HWND GethWnd() const { return m_hWnd; }
	bool getGridShow() const { return m_bShowGrid; }

	void setGridShow() { m_bShowGrid = !m_bShowGrid; }

public:
	bool Init(HINSTANCE hInst);
	int Run();

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};