#include "Monster.h"

Monster::Monster(RECT pos, MOVE_DIR mdr) : m_eMoveDir(mdr) 
{
    setPosition(pos);
    if (mdr == MOVE_DIR::MD_NONE) m_tImg.Load(L"Resource/Monster.bmp");
    else  m_tImg.Load(L"Resource/Monster.bmp");
}

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

    m_fCnt += fDeltaTime;
    if (m_fCnt >= 0.05) {
        m_fCnt = 0;
        m_iImageSprite++;
        if (m_iImageSprite > m_iMaxImageSprite) m_iImageSprite = 0;
    }

    return 0;
}

void Monster::render(HDC hdc)
{
    RECT t = (RECT)getPosition();

    FrameRect(hdc, &t, (HBRUSH)GetStockObject(WHITE_BRUSH));
    //Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);

    m_tImg.TransparentBlt(hdc, t.left, t.top, t.right - t.left, t.bottom - t.top,
        m_iImageSprite * 72, 72 * ((1 - static_cast<int>(m_eMoveDir)) >> 1), 70, 70, RGB(80, 40, 0));
}
