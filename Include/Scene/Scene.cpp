#include "Scene.h"
#include "../Core.h"



Scene::Scene(int iSceneNum) : m_iSceneNum(iSceneNum)
{
	m_iTileYLen = 18;

	switch (m_iSceneNum) {
	case -1:				// test Scene;
	{
		m_imgBackGround.Load(TEXT("Resource/bgTemp.png"));

		Player* p0 = new Player;
		Player* p1 = new Player;

		m_listPlayer.push_back(p0);
		m_listPlayer.push_back(p1);
		
		// load moving objs

		// static objs
		Step* s0 = new Step(SF_ALL, { 700,550,750,575 });
		Step* s1 = new Step(SF_YELLOW, { 800,550,850,575 });
		Step* s2 = new Step(SF_PUR, { 900,550,950,575 });

		m_listStep.push_back(s0);
		m_listStep.push_back(s1);
		m_listStep.push_back(s2);

		// tiles
		//m_listTiles;
		m_iTileXLen = 32;
		for (int i = 0; i < m_iTileYLen; ++i) {
			for (int j = 0; j < m_iTileXLen; ++j) {
				Tile* t;
				
				if (i == m_iTileYLen - 1) t = new Tile(TD_BLOCK);
				else if (i == m_iTileYLen - 2 && j >= m_iTileXLen / 2) t = new Tile(TD_BLOCK);
				else t = new Tile(TD_NON);

				
				m_listTiles.push_back(t);
			}
		}
		

		
	}
	break;
	}
}

Scene::~Scene()
{
	Safe_Delete_VecList(m_listPlayer);
	Safe_Delete_VecList(m_listTiles);
	Safe_Delete_VecList(m_listStep);
}

void Scene::init()
{
	// player init
	for (auto const& d : m_listPlayer) d->init();

}

void Scene::input(float fDeltaTile)
{
	for (auto const& d : m_listPlayer) d->input(fDeltaTile);
}

void Scene::update(float fDeltaTile)
{
	// 몬스터 업데이트(대충 몬스터 이동 이라는 뜻)
	//for (auto const& d : m_pMonsterList) d->Update();

}

void Scene::collision()
{
}

void Scene::render(HDC hdc)
{
	hdc = GetDC(Core::GetInst()->GethWnd());
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, Core::GetInst()->GetSize().cx, Core::GetInst()->GetSize().cy);

	SelectObject(memdc, hBitmap);

	// draw bg
	//Rectangle(memdc, -1, -1, Core::GetInst()->GetSize().cx + 1, Core::GetInst()->GetSize().cy + 1);
	m_imgBackGround.StretchBlt(memdc, { 0,0,Core::GetInst()->GetSize().cx ,Core::GetInst()->GetSize().cy }, SRCCOPY);

	// draw Tiles
	for (int i = 0; i < m_iTileYLen; ++i) {
		for (int j = 0; j < m_iTileXLen; ++j) {
			if (m_listTiles[m_iTileXLen * i + j]->getTile() == TD_BLOCK) 
				Rectangle(memdc, j * 40, i * 40, j * 40 + 40, i * 40 + 40);
		}
	}

	// draw obstacles
	for (auto const d : m_listStep) d->render(memdc);

	// draw monster
	//for (auto const d : m_pMonsterList) d->Render(memdc);
	// draw player
	for (auto const d : m_listPlayer) d->render(memdc);


	// grid Show
	if (Core::GetInst()->getGridShow()) {
		printf("Show Tile Grid\n");
	}


	BitBlt(hdc, 0, 0, Core::GetInst()->GetSize().cx, Core::GetInst()->GetSize().cy, memdc, 0, 0, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteDC(memdc);
	ReleaseDC(Core::GetInst()->GethWnd(), hdc);


}

