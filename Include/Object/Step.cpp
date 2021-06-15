#include "Step.h"

CImage Step::m_tStepImg;

bool Step::init()
{
	if (m_tStepImg.IsNull()) m_tStepImg.Load(L"Resource/imgsp.bmp");
	return true;
}

void Step::input(float fDeltaTime)
{
}

int Step::update(float fDeltaTime)
{
	return 0;
}

void Step::render(HDC hdc)
{
	if (m_bAlive == false && !m_ibuttonAliveCnt) return;

	RECT temp = (RECT)getPosition();

#ifdef DEBUG
	//HBRUSH hBrush = NULL, oBrush;
	//switch (m_eType) {
	//case STEP_FOR::SF_ALL:
	//case STEP_FOR::SF_PUR:
	//case STEP_FOR::SF_YELLOW:
	//	switch (m_eType) {
	//	case STEP_FOR::SF_ALL:		hBrush = CreateSolidBrush(RGB(150, 75, 0));		break;
	//	case STEP_FOR::SF_PUR:		hBrush = CreateSolidBrush(RGB(139, 75, 255));	break;
	//	case STEP_FOR::SF_YELLOW:	hBrush = CreateSolidBrush(RGB(255, 255, 0));	break;
	//	}
	//	oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//	Rectangle(hdc, temp.left, temp.top, temp.right, temp.bottom);

	//	SelectObject(hdc, oBrush);
	//	DeleteObject(hBrush);
	//	break;
	//}
	
#endif // DEBUG

	int t = 0;

	switch (m_eType) {
	case STEP_FOR::SF_ALL:		t = 1;	break;
	case STEP_FOR::SF_PUR:		t = 3;	break;
	case STEP_FOR::SF_YELLOW:	t = 2;	break;
	default:					t = 1;	break;
	}

	m_tStepImg.TransparentBlt(hdc, temp, { t * 72, 0,  t * 72 + 70, 29 }, RGB(80, 40, 0));

}
