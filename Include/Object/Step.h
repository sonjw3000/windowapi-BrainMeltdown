#pragma once
#include "Object.h"
#include "../Type.h"

class Step : public Object {
private:
	int m_iGroup;
	STEP_FOR m_eType;
	bool m_bAlive = true;

public:
	Step() = delete;
	Step(STEP_FOR sf, RECT pos) : m_eType(sf) { setPosition(pos); }
	Step(RECT pos, STEP_FOR sf, bool bAl, int group) : m_eType(sf), m_bAlive(bAl), m_iGroup(group) { setPosition(pos); }
	~Step() {}

public:
	STEP_FOR getType() { return m_eType; }
	bool isAlive() { return m_bAlive; }

public:
	void setAliveState(bool al) { m_bAlive = al; }
	void setGroup(int i) { m_iGroup = i; }

	int getGroup() { return m_iGroup; }
public:

public:
	virtual bool init();

	virtual void input(float fDeltaTime);
	virtual int update(float fDeltaTime);
	virtual void render(HDC hdc);
};

