#pragma once

#include "MovingObject.h"
#include "../GameManager/GameManager.h"

class Player : public MovingObject
{
private:
	friend class Object;

private:
	static int m_iPlayerCnt;
    int m_iPlayerNum;

public:
    Player();

public:
    ~Player();

private:
	bool m_bFalling = true;
	bool m_bSit = false;
	bool m_bSitComplete = false;
	bool m_bBeforeSit = false;

private:
	bool m_bLand = false;
	bool m_bBefJump = false;

private:
	float m_fJumpSpeed = 0;

private:
	MOVE_DIR m_eMoveDir = MOVE_DIR::MD_NONE;
	float m_fCnt = 0;

public:
	//bool isMoving() { return m_bMoving; }

private:
	int m_iSpriteY;
	int m_iImageSprite = 4;
	int m_iMaxImageSprite = 4;
	
private:

public:
	void updateSprite() { m_iImageSprite++; m_iImageSprite %= m_iMaxImageSprite; }

public:
	float getFallingSpeed() const { return m_fJumpSpeed; }

public:
	bool isFalling() { return m_bFalling; }
	int getPlayerNum() { return m_iPlayerNum; }
public:
	void notFalling() { m_bFalling = false; m_fJumpSpeed = 0; }
	void goFalling() { m_bFalling = true; }
	void hitCeil() {
		m_fJumpSpeed = 0;
	}

public:
	virtual bool init();

	virtual void input(float fDeltaTime);
	virtual int update(float fDeltaTime);
	virtual void render(HDC hdc);
};

