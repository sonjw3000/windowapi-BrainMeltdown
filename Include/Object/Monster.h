#pragma once
#include "MovingObject.h"


class Monster : public MovingObject {
private:
	MOVE_DIR m_eMoveDir = MOVE_DIR::MD_NONE;

public:
	Monster() = delete;
	Monster(RECT pos, MOVE_DIR mdr) : m_eMoveDir(mdr) { setPosition(pos); }
	~Monster();

public:
	void Reverse();

	MOVE_DIR getDir() const { return m_eMoveDir; }

public:
	virtual bool init();

	virtual void input(float fDeltaTime);
	virtual int update(float fDeltaTime);
	virtual void render(HDC hdc);

};
