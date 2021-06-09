#include "Step.h"

bool Step::init()
{
	return true;
}

void Step::input(float fDeltaTile)
{
}

int Step::update(float fDeltaTile)
{
	return 0;
}

void Step::render(HDC hdc)
{
	if (!m_bAlive) return;

	RECT temp = (RECT)getPosition();

#ifdef DEBUG
	HBRUSH hBrush = NULL, oBrush;
	switch (m_eType) {
	case SF_ALL:
	case SF_PUR:
	case SF_YELLOW:
		switch (m_eType) {
		case SF_ALL:	hBrush = CreateSolidBrush(RGB(150, 75, 0));		break;
		case SF_PUR:	hBrush = CreateSolidBrush(RGB(139, 75, 255));	break;
		case SF_YELLOW:	hBrush = CreateSolidBrush(RGB(255, 255, 0));	break;
		}
		oBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, temp.left, temp.top, temp.right, temp.bottom);

		SelectObject(hdc, oBrush);
		DeleteObject(hBrush);
		break;
	}
	
#endif // DEBUG


}
