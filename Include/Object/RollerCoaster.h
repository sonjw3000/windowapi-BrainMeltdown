#pragma once
#include "MovingObject.h"
class RollerCoaster : public MovingObject {
private:
	RECT m_tInitpos;
	RECT m_tMoveTo;

	int m_iGroup;
	int m_ibuttonAliveCnt;

	BOOL const m_bAlways;

	STEP_FOR m_eType;
	MOVE_DIR m_eDir;

private:
	static CImage m_tRCImg;

public:
	RollerCoaster() = delete;
	RollerCoaster(RECT pos, STEP_FOR t, int b, int g, RECT mt);
	~RollerCoaster();

public:
	int getGroup() { return m_iGroup; }
	STEP_FOR getType() { return m_eType; }

	void plusCount() { m_ibuttonAliveCnt++; }
	void minusCount() { m_ibuttonAliveCnt--; if (m_ibuttonAliveCnt < 0) m_ibuttonAliveCnt = 0; }
public:
	virtual bool init();

	virtual void input(float fDeltaTime);
	virtual int update(float fDeltaTime);
	virtual void render(HDC hdc);
};

