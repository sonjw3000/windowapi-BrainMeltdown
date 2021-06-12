#pragma once

#include "../Game.h"
#include "../Scene/Scene.h"

class GameManager
{
private:
	static GameManager* m_pInst;

public:
	static GameManager* GetInst() {
		if (!m_pInst) {
			m_pInst = new GameManager;
		}
		return m_pInst;
	}

	static void DestroyInst() {
		SAFE_DELETE(m_pInst);
	}

private:
	GameManager();
	~GameManager();

private:
	int m_iCurSceneNum = 0;
	class Scene* m_pScene;

private:
	bool CollideCheck(FRECT rt1, FRECT rt2);

public:
	void init();

public:
	void input(float fDeltaTime);
	void update(float fDeltaTime);
	void collision();
	void render(HDC hdc);


public:
	//void 
};

