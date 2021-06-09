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


void GameManager::input(float fDeltaTile)
{
	m_pScene->input(fDeltaTile);
}

void GameManager::update(float fDeltaTile)
{
	m_pScene->update(fDeltaTile);
}

void GameManager::collision()
{
	// 플레이어와 몬스터 충돌 확인
	bool bCollide = false;
	for (auto const dPlayer : m_pScene->m_listPlayer) {
		//for (auto const dMonster : m_pScene->m_lsitMonster) {
		//	if (CollideCheck(dPlayer->getPosition(), dMonster->getPosition())) bCollide = true;
		//}
		//if (bCollide) break;
	}

	// 플레이어와 타일맵 충돌 확인
	for (auto& dPlayer : m_pScene->m_listPlayer) {
		FRECT temp = dPlayer->getPosition();

		POINT tLeft = { floor((temp.left) / 40.0), floor((temp.bottom) / 40.0) };
		POINT tRight = { floor((temp.right) / 40.0), floor((temp.bottom) / 40.0) };

		// 바닥
		if (m_pScene->m_listTiles[tLeft.y * m_pScene->m_iTileXLen + tLeft.x]->getTile() == TD_BLOCK ||
			m_pScene->m_listTiles[tRight.y * m_pScene->m_iTileXLen + tRight.x]->getTile() == TD_BLOCK) {
			dPlayer->Move(0,  40 * tLeft.y - temp.bottom);
			//printf("%f", temp.bottom - 40 * tLeft.y);
			dPlayer->notFalling();
			bCollide = true;
		}
		else dPlayer->goFalling();						// NON
		
		// 벽?
		if (m_pScene->m_listTiles[tLeft.y * m_pScene->m_iTileXLen + tLeft.x - m_pScene->m_iTileXLen]->getTile() == TD_BLOCK ||
			m_pScene->m_listTiles[tRight.y * m_pScene->m_iTileXLen + tRight.x - m_pScene->m_iTileXLen]->getTile() == TD_BLOCK) dPlayer->goBackX();

	}

	//if (bCollide) {
	//	// gameOver;
	//	printf("End\n");
	//	return;
	//}


	// 플레이어와 장애물 충돌 확인		m_pStaticObjList
	bCollide = false;
	for (auto& dPlayer : m_pScene->m_listPlayer) {

		for (auto const dStep : m_pScene->m_listStep) {
			if (dStep->getType() - dPlayer->getPlayerNum()) {
				if (dPlayer->getPosition().IntersectRect(dStep->getPosition())) {
					// hit side
					if(dPlayer->getPosition().bottom > dStep->getPosition().bottom && dPlayer->getPosition().top < dStep->getPosition().top)
						dPlayer->goBackX();

					// hit top
					else if (dPlayer->getPosition().bottom - dStep->getPosition().bottom < 0) {
						while (dPlayer->getPosition().IntersectRect(dStep->getPosition())) dPlayer->Move(0, -0.1);

						dPlayer->notFalling();
						bCollide = true;
					}
					// hit bottom
					else if (dPlayer->getPosition().top - dStep->getPosition().bottom < 0) {
						while (dPlayer->getPosition().IntersectRect(dStep->getPosition())) dPlayer->Move(0, 0.1);

						dPlayer->hitCeil();
						bCollide = true;
					}
					printf("%f\n", dPlayer->getPosition().top - dStep->getPosition().bottom);
				}
			}
		}
		
	}

	// 플레이어와 플레이어 충돌 확인
	int playerCnt = 0;
	for (auto& dPlayer : m_pScene->m_listPlayer) {
		bool bCol = false;
		Player* pOtherPlayer = playerCnt ? m_pScene->m_listPlayer.front() : m_pScene->m_listPlayer.back(); 

		playerCnt++;

		if (m_pScene->m_listPlayer.front()->getPosition().IntersectRect(m_pScene->m_listPlayer.back()->getPosition())) {
			if (dPlayer->getPosition().IntersectRect(pOtherPlayer->getPosition())) {
				if (dPlayer->getPosition().bottom - pOtherPlayer->getPosition().bottom < -20) {
					while (dPlayer->getPosition().IntersectRect(pOtherPlayer->getPosition())) dPlayer->Move(0, -0.1);

					dPlayer->notFalling();
					bCollide = true;
				}
				else dPlayer->goBackX();
			}
			
		}
		//else 
	}



	// 몬스터와 장애물 충돌 확인
	//bCollide = false;
	//for (auto const dMonster : m_pMonsterList) {
	//	for (auto const dObs : m_pStaticObjList)
	//		if (CollideCheck(dMonster->getPosition(), dObs->getPosition())) bCollide = true;

	//	if (bCollide) {
	//		// monster change dircetb  
	//		printf("monster obs\n");
	//	}
	//}

	//if (!bCollide) {
	//	m_pScene->m_listPlayer.front()->goFalling();
	//	m_pScene->m_listPlayer.back()->goFalling();
	//}
}

void GameManager::render(HDC hdc)
{
	m_pScene->render(hdc);
}
