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

	case 1:				// test mapTool
	{
		m_imgBackGround.Load(TEXT("Resource/tempBGv2.bmp"));
		m_iTileImage.Load(TEXT("Resource/tempsprite.bmp"));

		Player* p0 = new Player;
		Player* p1 = new Player;

		//p0->setPosition({ 375,3500,400,3550 });
		//p1->setPosition({ 175,3500,200,3550 });

		//m_p0StartPos = { 375,1150,400,1200 };
		//m_p1StartPos = { 175,1150,200,1200 };

		m_p0StartPos = { 520,0,545,50 };
		m_p1StartPos = { 550,0,575,50 };

		p0->setPosition(m_p0StartPos);
		p1->setPosition(m_p1StartPos);


		m_listPlayer.push_back(p0);
		m_listPlayer.push_back(p1);


		fp = fopen("Scene/final.txt", "r");
		loadFile(fp);
	}
		break;

	case 999:				// game clear;
		m_iTileYLen = 18;
		m_iTileXLen = 32;
		m_imgBackGround.Load(TEXT("Resource/gameover.bmp"));
		break;
	}



}

Scene::~Scene()
{
	DeleteObject(hBitmap);
	m_iTileImage.Destroy();
	m_imgBackGround.Destroy();
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

void Scene::resetPlayerPos()
{
	m_listPlayer.front()->setPosition(m_p0StartPos);
	m_listPlayer.back()->setPosition(m_p1StartPos);
}

void Scene::init()
{
	// player init
	for (auto const& d : m_listPlayer) d->init();
	for (auto& d : m_listMonster) d->init();
	for (auto& d : m_listStep) d->init();
}

void Scene::input(float fDeltaTime)
{
	// player Move
	for (auto const& d : m_listPlayer) {
		d->input(fDeltaTime);

		while (d->getPosition().top < 0) {
			FRECT temp = d->getPosition();
			temp.OffsetRect(0, 1);
			d->setPosition(temp);
		}
	}
	// camera
	FPOINT pCenter;
	if (m_listPlayer.empty()) return;

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
	// 몬스터 업데이트(대충 몬스터 이동 이라는 뜻)
	for (auto const& d : m_listMonster) d->update(fDeltaTime);

	// 대충 롤러코스터 움직이라는 뜻
	for (auto& d : m_listRollerCoaster) d->update(fDeltaTime);
	
}

void Scene::collision()
{
}

void Scene::render(HDC hdc)
{
	hdc = GetDC(Core::GetInst()->GethWnd());
	HDC memdc = CreateCompatibleDC(hdc);
	if (hBitmap == NULL) hBitmap = CreateCompatibleBitmap(hdc, m_iTileXLen * TILESIZE, m_iTileYLen * TILESIZE);

	SIZE wndSize = Core::GetInst()->GetSize();
	SIZE imgSize = { m_imgBackGround.GetWidth(), m_imgBackGround.GetHeight() };

	SelectObject(memdc, hBitmap);

	// draw bg
	RECT imgRect = { 
	((m_CameraOffset.x * wndSize.cx) / (m_iTileXLen * TILESIZE)),
	((m_CameraOffset.y * wndSize.cy) / (m_iTileYLen * TILESIZE)),
	((m_CameraOffset.x + imgSize.cx) * wndSize.cx) / (m_iTileXLen * TILESIZE),
	((m_CameraOffset.y + imgSize.cy) * wndSize.cy) / (m_iTileYLen * TILESIZE) };
					 

	
	
	//m_imgBackGround.StretchBlt(memdc, { 0,0,m_iTileXLen * TILESIZE, m_iTileYLen * TILESIZE },  SRCCOPY);
	printf("%4d %4d %4d %4d\n", imgRect.left, imgRect.top, imgRect.right, imgRect.bottom);
	//m_imgBackGround.StretchBlt(memdc, 0, 0, m_iTileXLen * TILESIZE, m_iTileYLen * TILESIZE,
	//	imgRect.left, imgRect.top, imgRect.right - imgRect.left, imgRect.bottom - imgRect.top, SRCCOPY);

	//m_imgBackGround.StretchBlt(memdc, { static_cast<int>(m_CameraOffset.x), static_cast<int>(m_CameraOffset.y),
	//		static_cast<int>(m_CameraOffset.x) + wndSize.cx, static_cast<int>(m_CameraOffset.y) + wndSize.cy },imgRect, SRCCOPY);

	m_imgBackGround.BitBlt(memdc, { static_cast<int>(m_CameraOffset.x), static_cast<int>(m_CameraOffset.y),
		static_cast<int>(m_CameraOffset.x) + wndSize.cx, static_cast<int>(m_CameraOffset.y) + wndSize.cy },
		{ imgRect.left, imgRect.top }, SRCCOPY);

	//if End Scene return
	if (m_iSceneNum == 999) {
		BitBlt(hdc, 0, 0, wndSize.cx, wndSize.cy, memdc, m_CameraOffset.x, m_CameraOffset.y, SRCCOPY);

		//DeleteObject(hBitmap);
		DeleteDC(memdc);
		ReleaseDC(Core::GetInst()->GethWnd(), hdc);
		return;
	}

	// draw Tiles
	int x = m_CameraOffset.x / 40 - 1;
	int y = m_CameraOffset.y / 40 - 1;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	for (int i = y; i < y + 20; ++i) {
		for (int j = x; j < x + 34; ++j) {

			if (m_listTiles[m_iTileXLen * i + j]->getTile() == TILE_DATA::TD_BLOCK) {
				RECT temp = { j * TILESIZE, i * TILESIZE, (j + 1) * TILESIZE, (i + 1) * TILESIZE };

				m_iTileImage.TransparentBlt(memdc, j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE,
					72 * 5, 0, 70, 70, RGB(80, 40, 0));

				//FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
			} else if (m_listTiles[m_iTileXLen * i + j]->getTile() == TILE_DATA::TD_FLOOR) {
				RECT temp = { j * TILESIZE, i * TILESIZE, (j + 1) * TILESIZE, (i + 1) * TILESIZE };

				m_iTileImage.TransparentBlt(memdc, j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE,
					72 * 4, 0, 70, 70, RGB(80, 40, 0));

				//FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
			}
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


	BitBlt(hdc, 0, 0, wndSize.cx, wndSize.cy, memdc, m_CameraOffset.x, m_CameraOffset.y, SRCCOPY);

	//DeleteObject(hBitmap);
	DeleteDC(memdc);
	ReleaseDC(Core::GetInst()->GethWnd(), hdc);


}