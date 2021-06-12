#pragma once
#include "Object.h"
class Button : public Object {
private:
	bool m_bActive;
	bool m_bFixed;
	int m_iGroupControll;

public:
	Button() = delete;
	Button(RECT pos, bool isFixed, int group) : m_bFixed(isFixed), m_iGroupControll(group) { setPosition(pos); }
	~Button();

public:
	int getGroupCtrl() const { return m_iGroupControll; }
	bool isActive() const { return m_bActive; }
	bool isFixed() const { return m_bFixed; }

	void setActiveState(bool state) { m_bActive = state; }
public:
	virtual bool init();

	virtual void input(float fDeltaTime);
	virtual int update(float fDeltaTime);
	virtual void render(HDC hdc);
};

