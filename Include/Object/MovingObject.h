#pragma once
#include "Object.h"

class MovingObject : public Object {
protected:
	MovingObject();
	virtual ~MovingObject();

private:
	float m_fSpeed = 100.0;

protected:
	FRECT m_tBefPos;

public:
	void setSpeed(float speed) { m_fSpeed = speed; }
	float getSpeed() const { return m_fSpeed; }

public:
	void Move(float dx, float dy);
	void MoveXFromSpeed(MOVE_DIR eDir);
	void MoveYFromSpeed(MOVE_DIR eDir);

	void goBack();
	void goBackX();
	void goBackY();

public:
	virtual bool init() = 0;

	virtual void input(float fDeltaTime);
	virtual int update(float fDeltaTime);
	virtual void render(HDC hdc);
};