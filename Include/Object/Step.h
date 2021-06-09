#pragma once
#include "Object.h"
#include "../Type.h"

class Step : public Object {
private:
	STEP_FOR m_eType;
	bool m_bAlive = true;

public:
	Step() = delete;
	Step(STEP_FOR sf, RECT pos) : m_eType(sf) { setPosition(pos); }
	~Step() {}

public:
	STEP_FOR getType() { return m_eType; }
	bool isAlive() { return m_bAlive; }

public:
	void setAliveState(bool al) { m_bAlive = al; }

public:
	virtual bool init();

	virtual void input(float fDeltaTile);
	virtual int update(float fDeltaTile);
	virtual void render(HDC hdc);
};

