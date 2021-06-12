#include "Monster.h"

Monster::~Monster()
{
}

void Monster::Reverse()
{
    m_eMoveDir = m_eMoveDir * -1;
    m_tPosition = m_tBefPos;
}

bool Monster::init()
{
    setSpeed(50.0);
    m_tBefPos = m_tPosition;
    return true;
}

void Monster::input(float fDeltaTime)
{
}

int Monster::update(float fDeltaTime)
{
    m_tBefPos = m_tPosition;
    Move(static_cast<int>(m_eMoveDir) * getSpeed() * fDeltaTime, 0);

    return 0;
}

void Monster::render(HDC hdc)
{
    RECT rt = (RECT)getPosition();

    Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
}
