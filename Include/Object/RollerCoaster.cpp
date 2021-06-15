#include "RollerCoaster.h"

CImage RollerCoaster::m_tRCImg;

RollerCoaster::RollerCoaster(RECT pos, STEP_FOR t, int b, int g, RECT mt) 
	: m_tInitpos(pos), m_eType(t), m_ibuttonAliveCnt(0), m_iGroup(g), m_tMoveTo(mt), m_bAlways(b)
{ 
	
	if (m_tRCImg.IsNull()) m_tRCImg.Load(L"Resource/imgsp.bmp");
	
	setPosition(pos);

	if (m_tInitpos.left != m_tMoveTo.left) m_eDir = m_tInitpos.left > m_tMoveTo.left ? MOVE_DIR::MD_BACK : MOVE_DIR::MD_FRONT;
	else m_eDir = m_tInitpos.top > m_tMoveTo.top ? MOVE_DIR::MD_UP : MOVE_DIR::MD_DOWN;
}

RollerCoaster::~RollerCoaster()
{
	m_tRCImg.Destroy();
}

bool RollerCoaster::init()
{

	return true;
}

void RollerCoaster::input(float fDeltaTime)
{
}

int RollerCoaster::update(float fDeltaTime)
{
	//printf("%d\n", m_ibuttonAliveCnt);
	if (!(m_ibuttonAliveCnt + m_bAlways)) return 1;
	// move deltatime * speed
	if (-5 < static_cast<int>(m_eDir) && static_cast<int>(m_eDir) < 5) Move(fDeltaTime * getSpeed() * static_cast<int>(m_eDir), 0);
	else Move(0, fDeltaTime * getSpeed() * static_cast<int>(m_eDir) / 10);
	// if reached pos? speed * -1

	RECT temp = getPosition();
	if (-5 < static_cast<int>(m_eDir) && static_cast<int>(m_eDir) < 5) {
		POINT t = { static_cast<int>(m_eDir) == 1 ? m_tMoveTo.right : m_tMoveTo.left, (m_tMoveTo.bottom - m_tMoveTo.top) / 2 + m_tMoveTo.top };
		if (PtInRect(&temp, t)) {
			m_eDir = m_eDir * -1;

			temp = m_tMoveTo;
			m_tMoveTo = m_tInitpos;
			m_tInitpos = temp;
		}
	}
	else {
		POINT t = { (m_tMoveTo.right - m_tMoveTo.left) / 2 + m_tMoveTo.left, static_cast<int>(m_eDir) == 10 ? m_tMoveTo.bottom : m_tMoveTo.top };
		if (PtInRect(&temp, t)) {
			m_eDir = m_eDir * -1;

			temp = m_tMoveTo;
			m_tMoveTo = m_tInitpos;
			m_tInitpos = temp;
		}
	}
	return 0;
}

void RollerCoaster::render(HDC hdc)
{
	//if (!m_bAlive) return;

	RECT temp = (RECT)getPosition();

#ifdef DEBUG
	//HBRUSH hBrush = NULL, oBrush;

	//switch (m_eType) {
	//case STEP_FOR::SF_ALL:		hBrush = CreateSolidBrush(RGB(150, 75, 0));		break;
	//case STEP_FOR::SF_PUR:		hBrush = CreateSolidBrush(RGB(139, 75, 255));	break;
	//case STEP_FOR::SF_YELLOW:	hBrush = CreateSolidBrush(RGB(255, 255, 0));	break;
	//default:					hBrush = CreateSolidBrush(RGB(255, 255, 255));	break;
	//}
	//oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//Rectangle(hdc, temp.left, temp.top, temp.right, temp.bottom);

	//SelectObject(hdc, oBrush);
	//DeleteObject(hBrush);	

#endif // DEBUG
	int t = 0;

	switch (m_eType) {
	case STEP_FOR::SF_ALL:		t = 1;	break;
	case STEP_FOR::SF_PUR:		t = 3;	break;
	case STEP_FOR::SF_YELLOW:	t = 2;	break;
	default:					t = 1;	break;
	}

	m_tRCImg.TransparentBlt(hdc, temp, { t * 72, 0,  t * 72 + 70, 28 }, RGB(80, 40, 0));
}
