#pragma once
#include "MovingObject.h"


class Monster : public MovingObject {
private:
	MOVE_DIR m_eMoveDir = MOVE_DIR::MD_NONE;

private:
	static CImage m_tMonsterImg;

public:
	Monster() = delete;
	Monster(RECT pos, MOVE_DIR mdr);
	~Monster();

public:
	void Reverse();

	MOVE_DIR getDir() const { return m_eMoveDir; }

private:
	int m_iImageSprite = 0;
	int m_iMaxImageSprite = 7;
	float m_fCnt = 0;

public:
	virtual bool init();

	virtual void input(float fDeltaTime);
	virtual int update(float fDeltaTime);
	virtual void render(HDC hdc);

};
