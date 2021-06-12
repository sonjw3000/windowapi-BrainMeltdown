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
	m_tPosition.OffsetRect(m_fSpeed * static_cast<int>(eDir), 0.0);
}

void MovingObject::MoveYFromSpeed(MOVE_DIR eDir)
{
	m_tPosition.OffsetRect(0.0, m_fSpeed * static_cast<int>(eDir));
}

void MovingObject::goBack()
{
	m_tPosition = m_tBefPos;

	m_tBefPos = m_tPosition;
}

void MovingObject::goBackX()
{
	m_tPosition.left = m_tBefPos.left;
	m_tPosition.right = m_tBefPos.right;

	m_tBefPos = m_tPosition;
}

void MovingObject::goBackY()
{
	m_tPosition.top = m_tBefPos.top;
	m_tPosition.bottom = m_tBefPos.bottom;

	m_tBefPos = m_tPosition;
}

bool MovingObject::init()
{
	return false;
}

void MovingObject::input(float fDeltaTime)
{
	
}

int MovingObject::update(float fDeltaTime)
{
	return 0;
}


void MovingObject::render(HDC hdc)
{
}