#pragma once

#include "../Game.h"
#include "../Tile.h"
#include "../Object/Player.h"


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
	list<class Player* > m_lsitPlayer;
	// monster
	// roller coaster
	
	// static Objs
	// balpan
	// button

	// and camera;

public:
	void init();
	void input();
	void update();
	void collision();
	void render(HDC hdc);

};

