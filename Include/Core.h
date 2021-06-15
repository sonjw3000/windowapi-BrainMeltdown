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

private:
	LARGE_INTEGER m_Sec;
	LARGE_INTEGER m_Time;
	float m_fDeltaTime;
	float m_fTimeCnt = 0;

	bool m_bShowGrid = false;
	bool m_bGameLoop = true;

private:
	FMOD_SYSTEM* m_pSoundSystem;

public:
	FMOD_SYSTEM* getFmodSystem() { return m_pSoundSystem; }

public:
	SIZE GetSize() const { return m_tWndSize; }
	HWND GethWnd() const { return m_hWnd; }
	bool getGridShow() const { return m_bShowGrid; }
	bool getGameLoop() const { return m_bGameLoop; }

	void setGridShow() { m_bShowGrid = !m_bShowGrid; }
	void setGameLoopFalse() { m_bGameLoop = false; }

public:
	bool Init(HINSTANCE hInst);
	int Run();

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};