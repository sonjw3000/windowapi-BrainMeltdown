#include "MovingObject.h"

MovingObject::MovingObject()
{
}

MovingObject::~MovingObject()
{
}

void MovingObject::Move(float dx, float dy)
{
	m_tPosition.OffsetRect(dx, dy);
}

void MovingObject::MoveXFromSpeed(MOVE_DIR eDir)
{
	m_tPosition.OffsetRect(m_fSpeed * eDir, 0.0);
}

void MovingObject::MoveYFromSpeed(MOVE_DIR eDir)
{
	m_tPosition.OffsetRect(0.0, m_fSpeed * eDir);
}

void MovingObject::goBack()
{
	m_tPosition = m_tBefPos;
}

void MovingObject::goBackX()
{
	m_tPosition.left = m_tBefPos.left;
	m_tPosition.right = m_tBefPos.right;
}

void MovingObject::goBackY()
{
	m_tPosition.top = m_tBefPos.top;
	m_tPosition.bottom = m_tBefPos.bottom;
}

bool MovingObject::init()
{
	return false;
}

void MovingObject::input()
{
	
}

int MovingObject::update()
{
	return 0;
}


void MovingObject::render(HDC hdc)
{
}