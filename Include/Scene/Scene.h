#pragma once

#include "../Game.h"
#include "../Tile.h"
#include "../Object/Player.h"
#include "../Object/Step.h"
#include "../Object/Monster.h"
#include "../Object/RollerCoaster.h"
#include "../Object/Button.h"

// It has 
class Scene {
private:
	friend class GameManager;

public:
	Scene() = delete;
	Scene(int iSceneNum);
	~Scene();

private:
	int m_iSceneNum;
	CImage m_imgBackGround;
	CImage m_iTileImage;

	// movingObjs
	list<class Player* > m_listPlayer;
	// monster
	vector<class Monster*> m_listMonster;
	// roller coaster
	vector<class RollerCoaster*> m_listRollerCoaster;

	// static Objs
	// balpan
	list<class Step*> m_listStep;
	// button
	vector<class Button*> m_listButton;

	// tiles;
	vector<class Tile*> m_listTiles;
	int m_iTileXLen;
	int m_iTileYLen;

	// and camera;
	FPOINT m_CameraOffset = { 0.0,0.0 };
	const SIZE m_CameraRectSize = { 800,800 };
	float dx;
	//RECT m_rtCamera;

private:
	RECT m_p0StartPos;
	RECT m_p1StartPos;

private:
	HBITMAP hBitmap = NULL;

private:
	bool loadFile(FILE* fp);

public:
	void resetPlayerPos();

public:
	void init();
	void input(float fDeltaTime);
	void update(float fDeltaTime);
	void collision();
	void render(HDC hdc);

};

