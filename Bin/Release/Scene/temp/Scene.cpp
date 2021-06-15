#include "Scene.h"
#include "Core.h"



Scene::Scene(int iSceneNum) : m_iSceneNum(iSceneNum)
{
	switch (m_iSceneNum) {
	case -1:				// test Scene;
	{
		m_imgBackGround.Load(TEXT("../Resource/bgTemp.png"));

		Player* p0 = new Player;
		Player* p1 = new Player;

		m_lsitPlayer.push_back(p0);
		m_lsitPlayer.push_back(p1);
		
		// load moving objs

		// static objs

		// tile

	}
	break;
	}
}

Scene::~Scene()
{
	Safe_Delete_VecList(m_lsitPlayer);

}

void Scene::init()
{
	// player init
	for (auto const& d : m_lsitPlayer) d->init();

}

void Scene::input()
{
	for (auto const& d : m_lsitPlayer) d->input();
}

void Scene::update()
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
	Rectangle(memdc, -1, -1, Core::GetInst()->GetSize().cx + 1, Core::GetInst()->GetSize().cy + 1);
	m_imgBackGround.StretchBlt(memdc, { 0,0,Core::GetInst()->GetSize().cx ,Core::GetInst()->GetSize().cy }, SRCCOPY);
	// draw obstacles

	// draw monster
	//for (auto const d : m_pMonsterList) d->Render(memdc);
	// draw player
	for (auto const d : m_lsitPlayer) d->render(memdc);


	BitBlt(hdc, 0, 0, Core::GetInst()->GetSize().cx, Core::GetInst()->GetSize().cy, memdc, 0, 0, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteDC(memdc);
	ReleaseDC(Core::GetInst()->GethWnd(), hdc);
}

