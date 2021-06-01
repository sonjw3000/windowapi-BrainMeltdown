#include "GameManager.h"
#include "../Core.h"

//#include "../Game.h"

GameManager* GameManager::m_pInst = NULL;

GameManager::GameManager() : m_pScene(NULL)
{
}

GameManager::~GameManager()
{
	// clear;
	SAFE_DELETE(m_pScene);
}

bool GameManager::CollideCheck(FRECT rt1, FRECT rt2)
{
	return rt1.IntersectRect(rt2);
}

void GameManager::init()
{
	m_iCurSceneNum = -1;
	m_pScene = new Scene(m_iCurSceneNum);

	m_pScene->init();
}


void GameManager::input()
{
	m_pScene->input();
}

void GameManager::update()
{
	m_pScene->update();
}

void GameManager::collision()
{
	// �÷��̾�� ���� �浹 Ȯ��
	bool bCollide = false;
	for (auto const dPlayer : m_pScene->m_lsitPlayer) {
		//for (auto const dMonster : m_pScene->m_lsitMonster) {
		//	if (CollideCheck(dPlayer->getPosition(), dMonster->getPosition())) bCollide = true;
		//}
		//if (bCollide) break;
	}

	// �÷��̾�� Ÿ�ϸ� �浹 Ȯ��
	int now = 0;
	for (auto& dPlayer : m_pScene->m_lsitPlayer) {
		FRECT temp = dPlayer->getPosition();

		POINT tPos = { floor((temp.left) / 40.0), ceil((temp.bottom - 40.0) / 40.0) };

		//POINT t = { floor(tPos.x) / m_pScene->m_iTileXLen, ceil(temp.bottom / 40.0) };

		if (m_pScene->m_listTiles[tPos.y * m_pScene->m_iTileXLen + tPos.x]->getTile() == TD_BLOCK) dPlayer->notFalling();
		else {
			dPlayer->goFalling();
		}
	}

	if (bCollide) {
		// gameOver;
		printf("End\n");
		return;
	}


	// �÷��̾�� ��ֹ� �浹 Ȯ��		m_pStaticObjList
	bCollide = false;
	for (auto const dPlayer : m_pScene->m_lsitPlayer) {
		//for (auto const dObs : m_pStaticObjList) 
		//	if (CollideCheck(dPlayer->getPosition(), dObs->getPosition())) bCollide = true;
		//
		//if (bCollide) {
		//	// player cant move
		//	printf("player obs\n");
		//}
	}

	// �÷��̾�� �÷��̾� �浹 Ȯ��
	bCollide = false;
	if (CollideCheck(m_pScene->m_lsitPlayer.front()->getPosition(), m_pScene->m_lsitPlayer.back()->getPosition())) bCollide = true;
	if (bCollide) {
		// player cant move
		FRECT t1 = m_pScene->m_lsitPlayer.front()->getPosition();
		FRECT t2 = m_pScene->m_lsitPlayer.back()->getPosition();
		printf("%.2f %.2f %.2f %.2f\t\t%.2f %.2f %.2f %.2f\n", t1.left, t1.top, t1.right, t1.bottom, t2.left, t2.top, t2.right, t2.bottom);


		if (m_pScene->m_lsitPlayer.front()->getPosition().bottom > m_pScene->m_lsitPlayer.back()->getPosition().top) 
			m_pScene->m_lsitPlayer.front()->goBackY();
		else if (m_pScene->m_lsitPlayer.back()->getPosition().bottom > m_pScene->m_lsitPlayer.front()->getPosition().top)
			m_pScene->m_lsitPlayer.back()->goBackY();
		else {
			m_pScene->m_lsitPlayer.front()->goBackX();
			m_pScene->m_lsitPlayer.back()->goBackX();
		}
		//printf("player collide\n");

	}


	// ���Ϳ� ��ֹ� �浹 Ȯ��
	bCollide = false;
	//for (auto const dMonster : m_pMonsterList) {
	//	for (auto const dObs : m_pStaticObjList)
	//		if (CollideCheck(dMonster->getPosition(), dObs->getPosition())) bCollide = true;

	//	if (bCollide) {
	//		// monster change dircetb  
	//		printf("monster obs\n");
	//	}
	//}
}

void GameManager::render(HDC hdc)
{
	m_pScene->render(hdc);
}
