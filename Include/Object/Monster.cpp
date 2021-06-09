#include "Monster.h"

Monster::~Monster()
{
}

bool Monster::init()
{
    setSpeed(1.f);
    return true;
}

void Monster::input(float fDeltaTile)
{
}

int Monster::update(float fDeltaTile)
{
    Move(m_eMoveDir * getSpeed(), 0);
    return 0;
}

void Monster::render(HDC hdc)
{
    RECT rt = (RECT)getPosition();

    Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
}
