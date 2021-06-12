#include "Button.h"

Button::~Button()
{
}

bool Button::init()
{
    m_bActive = false;
    return true;
}

void Button::input(float fDeltaTime)
{
}

int Button::update(float fDeltaTime)
{
    return 0;
}

void Button::render(HDC hdc)
{
    RECT t = getPosition();
    if (!m_bActive) Rectangle(hdc, t.left, t.top, t.right, t.bottom);
    else Rectangle(hdc, t.left, t.top + 10, t.right, t.bottom);              // draw smal
}
