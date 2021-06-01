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
	float m_bJumpSpeed = 0;

public:
	bool isFalling() { return m_bFalling; }

public:
	void notFalling() { m_bFalling = false; m_bJumpSpeed = 0; }
	void goFalling() { m_bFalling = true; }
public:
	virtual bool init();

	virtual void input();
	virtual int update();
	virtual void render(HDC hdc);
};

