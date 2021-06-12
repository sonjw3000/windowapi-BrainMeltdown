#include "Scene.h"
#include "../Core.h"



Scene::Scene(int iSceneNum) : m_iSceneNum(iSceneNum)
{
	m_iTileYLen = 18;
	FILE* fp = NULL;
	// set player position and background
	switch (m_iSceneNum) {
	case -10:				// test Scene;
	{
		m_iTileXLen = 32;


		m_imgBackGround.Load(TEXT("Resource/bgTemp.png"));

		Player* p0 = new Player;
		Player* p1 = new Player;

		m_listPlayer.push_back(p0);
		m_listPlayer.push_back(p1);
		
		// load moving objs

		// static objs
		Step* s0 = new Step(STEP_FOR::SF_ALL, { 700,550,750,575 });
		Step* s1 = new Step(STEP_FOR::SF_YELLOW, { 800,550,850,575 });
		Step* s2 = new Step(STEP_FOR::SF_PUR, { 900,550,950,575 });

		m_listStep.push_back(s0);
		m_listStep.push_back(s1);
		m_listStep.push_back(s2);

		// tiles
		//m_listTiles;
		for (int i = 0; i < m_iTileYLen; ++i) {
			for (int j = 0; j < m_iTileXLen; ++j) {
				Tile* t;
				
				if (i == m_iTileYLen - 1) t = new Tile(TILE_DATA::TD_BLOCK);
				else if (i == m_iTileYLen - 2 && j >= m_iTileXLen / 2) t = new Tile(TILE_DATA::TD_BLOCK);
				else t = new Tile(TILE_DATA::TD_NON);

				
				m_listTiles.push_back(t);
			}
		}
		

		
	}
	break;

	case -1:				// test mapTool
	{
		m_imgBackGround.Load(TEXT("Resource/bgTemp.png"));

		Player* p0 = new Player;
		Player* p1 = new Player;

		m_listPlayer.push_back(p0);
		m_listPlayer.push_back(p1);


		fp = fopen("Scene/01_TestScene.txt", "r");
		


		// get x,y size
		//fscanf(fp, "%d %d", &m_iTileXLen, &m_iTileYLen);

		//// get tiles
		//for (int i = 0; i < m_iTileYLen; ++i) {
		//	for (int j = 0; j < m_iTileXLen; ++j) {
		//		TILE_DATA temp;
		//		fscanf(fp, "%d", &temp);


		//		Tile* t = new Tile(temp);


		//		m_listTiles.push_back(t);
		//	}
		//}


		//fclose(fp);

	}
		break;

	}

	loadFile(fp);


}

Scene::~Scene()
{
	Safe_Delete_VecList(m_listPlayer);
	Safe_Delete_VecList(m_listTiles);
	Safe_Delete_VecList(m_listStep);
	Safe_Delete_VecList(m_listButton);
	Safe_Delete_VecList(m_listRollerCoaster);
	Safe_Delete_VecList(m_listMonster);
}

bool Scene::loadFile(FILE* fp)
{
	if (fp == NULL) return false;

	// get x,y size
	fscanf(fp, "%d %d", &m_iTileXLen, &m_iTileYLen);

	// get tiles
	for (int i = 0; i < m_iTileYLen; ++i) {
		for (int j = 0; j < m_iTileXLen; ++j) {
			TILE_DATA temp;
			fscanf(fp, "%d", &temp);

			Tile* t = new Tile(temp);

			m_listTiles.push_back(t);
		}
	}

	int size;
	// monster
	fscanf(fp, "%d", &size);
	for (int i = 0; i < size; ++i) {
		RECT t;
		MOVE_DIR md;
		fscanf(fp, "%d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &md);

		Monster* temp = new Monster(t, md);
		//printf("%d %d %d %d\n", t.left, t.top, t.right, t.bottom);
		m_listMonster.push_back(temp);
	}

	// rollercoaster
	fscanf(fp, "%d", &size);
	for (int i = 0; i < size; ++i) {
		RECT t;
		STEP_FOR sf;
		int b;
		int g;
		RECT mt;
		fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &sf, &b, &g, &mt.left, &mt.top, &mt.right, &mt.bottom);

		//RollerCoaster(RECT pos, STEP_FOR t, BOOL b, int g, RECT mt);
		RollerCoaster* temp = new RollerCoaster(t, sf, b, g, mt);

		m_listRollerCoaster.push_back(temp);
	}
	
	// step
	fscanf(fp, "%d", &size);
	for (int i = 0; i < size; ++i) {
		RECT t;
		STEP_FOR sf;
		int group;
		BOOL alive;
		fscanf(fp, "%d %d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &sf, &alive, &group);

		Step* temp = new Step(t, sf, alive, group);

		m_listStep.push_back(temp);
	}

	// button
	fscanf(fp, "%d", &size);
	for (int i = 0; i < size; ++i) {
		RECT t;
		BOOL fixed;
		int g;
		fscanf(fp, "%d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &fixed, &g);

		Button* temp = new Button(t, fixed, g);

		m_listButton.push_back(temp);
	}

	fclose(fp);
	return true;
}

void Scene::init()
{
	// player init
	for (auto const& d : m_listPlayer) d->init();
	for (auto& d : m_listMonster) d->init();
}

void Scene::input(float fDeltaTime)
{
	for (auto const& d : m_listPlayer) d->input(fDeltaTime);

	// camera
	FPOINT pCenter;
	
	pCenter.x = (m_listPlayer.front()->getPosition().left + m_listPlayer.back()->getPosition().left) / 2;
	pCenter.y = (m_listPlayer.front()->getPosition().bottom + m_listPlayer.back()->getPosition().bottom) / 2;

	m_CameraOffset.x = pCenter.x - m_CameraRectSize.cx / 2;
	m_CameraOffset.y = pCenter.y - m_CameraRectSize.cy / 2;

	if (m_CameraOffset.x < 0) m_CameraOffset.x = 0;
	else if (m_CameraOffset.x > TILESIZE * m_iTileXLen - m_CameraRectSize.cx) m_CameraOffset.x = TILESIZE * m_iTileXLen - m_CameraRectSize.cx;

	if (m_CameraOffset.y < 0) m_CameraOffset.y = 0;
	else if (m_CameraOffset.y > TILESIZE * m_iTileYLen - m_CameraRectSize.cy) m_CameraOffset.y = TILESIZE * m_iTileYLen - m_CameraRectSize.cy;
}

void Scene::update(float fDeltaTime)
{
	// ���� ������Ʈ(���� ���� �̵� �̶�� ��)
	for (auto const& d : m_listMonster) d->update(fDeltaTime);

	// ���� �ѷ��ڽ��� �����̶�� ��
	for (auto& d : m_listRollerCoaster) d->update(fDeltaTime);
	
}

void Scene::collision()
{
}

void Scene::render(HDC hdc)
{
	hdc = GetDC(Core::GetInst()->GethWnd());
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, m_iTileXLen * TILESIZE, m_iTileYLen * TILESIZE);

	SelectObject(memdc, hBitmap);

	// draw bg
	//Rectangle(memdc, -1, -1, Core::GetInst()->GetSize().cx + 1, Core::GetInst()->GetSize().cy + 1);
	m_imgBackGround.StretchBlt(memdc, { 0,0,Core::GetInst()->GetSize().cx ,Core::GetInst()->GetSize().cy }, SRCCOPY);

	// draw Tiles
	for (int i = 0; i < m_iTileYLen; ++i) {
		for (int j = 0; j < m_iTileXLen; ++j) {
			if (m_listTiles[m_iTileXLen * i + j]->getTile() == TILE_DATA::TD_BLOCK) 
				Rectangle(memdc, j * 40, i * 40, j * 40 + 40, i * 40 + 40);
		}
	}

	// draw step, rollercoaster
	for (auto const d : m_listStep) d->render(memdc);
	for (auto const d : m_listRollerCoaster) d->render(memdc);

	// draw button
	for (auto const d : m_listButton) d->render(memdc);

	// draw monster
	for (auto const d : m_listMonster) d->render(memdc);
	// draw player
	for (auto const d : m_listPlayer) d->render(memdc);


	BitBlt(hdc, 0, 0, Core::GetInst()->GetSize().cx, Core::GetInst()->GetSize().cy, memdc, m_CameraOffset.x, m_CameraOffset.y, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteDC(memdc);
	ReleaseDC(Core::GetInst()->GethWnd(), hdc);


}