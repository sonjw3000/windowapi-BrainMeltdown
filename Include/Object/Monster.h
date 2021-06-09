#pragma once
#include "MovingObject.h"


class Monster : public MovingObject {
private:
	MOVE_DIR m_eMoveDir = MD_NONE;

public:
	Monster() = delete;
	Monster(RECT pos, MOVE_DIR mdr) : m_eMoveDir(mdr) { setPosition(pos); }
	~Monster();

public:
	virtual bool init();

	virtual void input(float fDeltaTile);
	virtual int update(float fDeltaTile);
	virtual void render(HDC hdc);

};
