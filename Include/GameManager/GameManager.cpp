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

void GameManager::sceneChange(int iSceneNum)
{
	m_iCurSceneNum = iSceneNum;
	SAFE_DELETE(m_pScene);
	m_pScene = new Scene(m_iCurSceneNum);
	
	m_pScene->init();
}

void GameManager::init()
{
	// test scene == -1
	m_iCurSceneNum = 1;
	m_pScene = new Scene(m_iCurSceneNum);

	m_pScene->init();
}


void GameManager::input(float fDeltaTime)
{
	m_pScene->input(fDeltaTime);
}

void GameManager::update(float fDeltaTime)
{
	if (m_pScene->m_iSceneNum != 999) m_pScene->update(fDeltaTime);
}

void GameManager::collision()
{
	if (m_pScene->m_iSceneNum == 999) return;

	RECT camRect = { m_pScene->m_CameraOffset.x,m_pScene->m_CameraOffset.y,m_pScene->m_CameraOffset.x + 1280, m_pScene->m_CameraOffset.y + 720 };
	// 플레이어와 몬스터 충돌 확인
	bool bCollide = false;
	for (auto const dPlayer : m_pScene->m_listPlayer) {
		for (auto const dMonster : m_pScene->m_listMonster) {
			if (!dMonster->getPosition().IntersectRect(camRect)) continue;
			if (CollideCheck(dPlayer->getPosition(), dMonster->getPosition())) {
				sceneChange(m_iCurSceneNum);
				return;
			}
		}
	}

	// 플레이어와 타일맵 충돌 확인
	for (auto& dPlayer : m_pScene->m_listPlayer) {
		FRECT temp = dPlayer->getPosition();

		POINT tLeft = { floor((temp.left) / 40.0), floor((temp.bottom) / 40.0) };
		POINT tRight = { floor((temp.right) / 40.0), floor((temp.bottom) / 40.0) };

		// 바닥
		int LB = tLeft.y * m_pScene->m_iTileXLen + tLeft.x;
		int RB = tRight.y * m_pScene->m_iTileXLen + tRight.x;
		TILE_DATA leftBottom = (0 <= LB && LB < m_pScene->m_listTiles.size()) ? m_pScene->m_listTiles[LB]->getTile() : TILE_DATA::TD_NON;
		TILE_DATA rightBottom = (0 <= RB && RB < m_pScene->m_listTiles.size()) ? m_pScene->m_listTiles[RB]->getTile() : TILE_DATA::TD_NON;

		if (leftBottom == TILE_DATA::TD_BLOCK || rightBottom == TILE_DATA::TD_BLOCK ||
			leftBottom == TILE_DATA::TD_FLOOR || rightBottom == TILE_DATA::TD_FLOOR) {
			dPlayer->Move(0, 40 * tLeft.y - temp.bottom);
			//printf("%f", temp.bottom - 40 * tLeft.y);
			dPlayer->notFalling();

		}
		else if (leftBottom == TILE_DATA::TD_SPIKE || rightBottom == TILE_DATA::TD_SPIKE) {
			// gameOver;
			m_pScene->resetPlayerPos();
			sceneChange(m_iCurSceneNum);
			//sceneChange(999);
			//Core::GetInst()->setGameLoopFalse();
			return;
		}
		else if (leftBottom == TILE_DATA::TD_GOAL || rightBottom == TILE_DATA::TD_GOAL) {
			sceneChange(999);
			return;
		}
		else dPlayer->goFalling();						// NON
		
		// 벽?
		LB = tLeft.y * m_pScene->m_iTileXLen + tLeft.x - m_pScene->m_iTileXLen;
		RB = tRight.y * m_pScene->m_iTileXLen + tRight.x - m_pScene->m_iTileXLen;

		
		if (((0 <= LB && LB < m_pScene->m_listTiles.size()) && 
			(0 <= RB && RB < m_pScene->m_listTiles.size())) &&
			m_pScene->m_listTiles[LB]->getTile() == TILE_DATA::TD_BLOCK ||
			m_pScene->m_listTiles[RB]->getTile() == TILE_DATA::TD_BLOCK ||
			m_pScene->m_listTiles[LB]->getTile() == TILE_DATA::TD_FLOOR ||
			m_pScene->m_listTiles[RB]->getTile() == TILE_DATA::TD_FLOOR)
			dPlayer->goBackX();
		else if (((0 <= LB && LB < m_pScene->m_listTiles.size()) &&
			(0 <= RB && RB < m_pScene->m_listTiles.size())) &&
			m_pScene->m_listTiles[LB]->getTile() == TILE_DATA::TD_GOAL ||
			m_pScene->m_listTiles[RB]->getTile() == TILE_DATA::TD_GOAL) {
			sceneChange(999);
			return;
		}

	}

	//if (bCollide) {
	//	// gameOver;
	//	printf("End\n");
	//	return;
	//}


	// 플레이어와 장애물 충돌 확인		(step, rollercoaster, button)
	bCollide = false;
	for (auto& dPlayer : m_pScene->m_listPlayer) {
		// step
		for (auto const dStep : m_pScene->m_listStep) {
			if (!dStep->getPosition().IntersectRect(camRect) || !(dStep->isAlive() + dStep->getCount())) continue;

			if (static_cast<int>(dStep->getType()) - dPlayer->getPlayerNum()) {
				if (dPlayer->getPosition().IntersectRect(dStep->getPosition())) {
					// hit side
					if(dPlayer->getPosition().bottom > dStep->getPosition().bottom && dPlayer->getPosition().top < dStep->getPosition().top)
						dPlayer->goBackX();

					// hit top
					else if (dPlayer->getPosition().bottom - dStep->getPosition().bottom < 0) {
						while (dPlayer->getPosition().IntersectRect(dStep->getPosition())) dPlayer->Move(0, -0.1);

						dPlayer->Move(0, 0.1);
						dPlayer->notFalling();
						bCollide = true;
					}
					// hit bottom
					else if (dPlayer->getPosition().top - dStep->getPosition().bottom < 0) {
						while (dPlayer->getPosition().IntersectRect(dStep->getPosition())) dPlayer->Move(0, 0.1);

						dPlayer->hitCeil();
						bCollide = true;
					}
					//printf("%f\n", dPlayer->getPosition().top - dStep->getPosition().bottom);
				}
			}
		}
		
		// rollercoaster
		for (auto const dRCoaster : m_pScene->m_listRollerCoaster) {
			if (!dRCoaster->getPosition().IntersectRect(camRect)) continue;

			if (static_cast<int>(dRCoaster->getType()) - dPlayer->getPlayerNum()) {
				if (dPlayer->getPosition().IntersectRect(dRCoaster->getPosition())) {
					// hit side
					if (dPlayer->getPosition().bottom > dRCoaster->getPosition().bottom && dPlayer->getPosition().top < dRCoaster->getPosition().top)
						dPlayer->goBackX();

					// hit top
					else if (dPlayer->getPosition().bottom - dRCoaster->getPosition().bottom < 0) {
						while (dPlayer->getPosition().IntersectRect(dRCoaster->getPosition())) dPlayer->Move(0, -0.1);

						dPlayer->Move(0, 0.1);
						dPlayer->notFalling();
						bCollide = true;
					}
					// hit bottom
					else if (dPlayer->getPosition().top - dRCoaster->getPosition().bottom < 0) {
						while (dPlayer->getPosition().IntersectRect(dRCoaster->getPosition())) dPlayer->Move(0, 0.1);

						dPlayer->hitCeil();
						bCollide = true;
					}
					//printf("%f\n", dPlayer->getPosition().top - dRCoaster->getPosition().bottom);
				}
			}
		}
	}
	
	// button
	// turn off everything
	for (auto& d : m_pScene->m_listRollerCoaster) d->minusCount();
	for (auto& d : m_pScene->m_listStep) if (!d->isAlive()) d->minusCount();
	for (auto& dButton : m_pScene->m_listButton) {
		
		for (auto& dPlayer : m_pScene->m_listPlayer) {
			if (dPlayer->getPosition().IntersectRect(dButton->getPosition())) {
				//if (dButton->isActive()) break;
				dButton->setActiveState(true);

				// turn on everything
				for (auto& d : m_pScene->m_listRollerCoaster) if (d->getGroup() == dButton->getGroupCtrl())
					d->plusCount();
				for (auto& d : m_pScene->m_listStep) if (!d->isAlive() &&d->getGroup() == dButton->getGroupCtrl())
					d->plusCount();
				break;
			}
			else {
				if (dButton->isFixed()) break;
				dButton->setActiveState(false);

				// turn off everything
				//for (auto& d : m_pScene->m_listRollerCoaster) if (d->getGroup() == dButton->getGroupCtrl()) d->minusCount();
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

					dPlayer->Move(0, 0.1);
					dPlayer->notFalling();
					bCollide = true;
				}
				else dPlayer->goBackX();
			}
			
		}
		//else 
	}

	// 몬스터와 몬스터 충돌
	for (auto iter = m_pScene->m_listMonster.begin(); iter != m_pScene->m_listMonster.end(); ++iter) {
		for (auto inIt = (iter + 1); inIt != m_pScene->m_listMonster.end(); ++inIt) {
			if ((*iter)->getPosition().IntersectRect((*inIt)->getPosition())) {
				(*iter)->goBack();
				(*inIt)->goBack();

				(*iter)->Reverse();
				(*inIt)->Reverse();
				break;
			}
		}
	}
	// 몬스터와 타일맵 충돌(벽)
	for (auto d : m_pScene->m_listMonster) {
		POINT t;
		switch (d->getDir()) {
		case MOVE_DIR::MD_BACK:		t = { static_cast<LONG>(d->getPosition().left), static_cast<LONG>(d->getPosition().top) };	break;
		case MOVE_DIR::MD_FRONT:	t = { static_cast<LONG>(d->getPosition().right), static_cast<LONG>(d->getPosition().top) };	break;
		default: continue;
		}
		t.x = t.x / 40;
		t.y = t.y / 40;
			//tLeft.y* m_pScene->m_iTileXLen + tLeft.x
		if (m_pScene->m_listTiles[t.y * m_pScene->m_iTileXLen + t.x]->getTile() == TILE_DATA::TD_BLOCK || 
			m_pScene->m_listTiles[t.y * m_pScene->m_iTileXLen + t.x]->getTile() == TILE_DATA::TD_FLOOR)
			d->Reverse();		
	}

	//if (!bCollide) {
	//	m_pScene->m_listPlayer.front()->goFalling();
	//	m_pScene->m_listPlayer.back()->goFalling();
	//}
}

void GameManager::render(HDC hdc)
{
	m_pScene->render(hdc);
}
