#pragma once

#include "../Game.h"
#include "../Tile.h"
#include "../Object/Player.h"
#include "../Object/Step.h"


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

	// movingObjs
	list<class Player* > m_listPlayer;
	// monster
	// roller coaster
	
	// static Objs
	// balpan
	list<class Step*> m_listStep;
	// button

	// tiles;
	vector<class Tile*> m_listTiles;
	int m_iTileXLen;
	int m_iTileYLen;

	// and camera;

public:
	void init();
	void input(float fDeltaTile);
	void update(float fDeltaTile);
	void collision();
	void render(HDC hdc);

};

