#include "Player.h"
#include "Object.h"

int Player::m_iPlayerCnt = 0;

Player::Player() : m_iPlayerNum(m_iPlayerCnt++)
{
}

Player::~Player()
{
	m_iPlayerCnt--;
}


bool Player::init()
{
	if (m_iPlayerNum == -1) return false;

	setTag("Player");
	setSpeed(250);

	if (m_iPlayerNum == 0) {
		setPosition({ 375,350,400,400 });
		m_tImg.Load(L"Resource/playerBora.png");
	}
	else {
		setPosition({ 175,150,200,200 });
		m_tImg.Load(L"Resource/p1temp.png");

	}


	return true;
}

void Player::input(float fDeltaTile)
{
	m_tBefPos = m_tPosition;

	Object::input(fDeltaTile);
	
	bool bMoved = false;

	if (m_iPlayerNum == 0) {
		if (GetAsyncKeyState(VK_LEFT)) {
			Move(-getSpeed() * fDeltaTile, 0);
			m_eMoveDir = MD_BACK;
			bMoved = true;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			Move(getSpeed() * fDeltaTile, 0);
			m_eMoveDir = MD_FRONT;
			bMoved = true;
		}
		if (!m_bFalling) if (GetAsyncKeyState(VK_UP)) {
			m_bFalling = true;
			m_bJumpSpeed = -500;
		}

		//if (GetAsyncKeyState(VK_DOWN)) Move(0, getSpeed());
	}
	else {
		if (GetAsyncKeyState('A')) {
			Move(-getSpeed() * fDeltaTile, 0);
			m_eMoveDir = MD_BACK;
			bMoved = true;
		}
		if (GetAsyncKeyState('D')) {
			Move(getSpeed() * fDeltaTile, 0);
			m_eMoveDir = MD_FRONT;
			bMoved = true;
		}
		if (!m_bFalling) if (GetAsyncKeyState('W')) {
			m_bFalling = true;
			m_bJumpSpeed = -500;
		}
	}

	if (m_bFalling){
		Move(0.0, m_bJumpSpeed * fDeltaTile);

		m_bJumpSpeed += 500 * fDeltaTile * 3;
		if (m_bJumpSpeed >= 500) m_bJumpSpeed = 500;
	}

	if (bMoved) {
		m_fCnt += fDeltaTile;
		if (m_fCnt >= 0.075) {
			m_iImageSprite++;
			m_fCnt = 0;
		}
		m_iImageSprite %= m_iMaxImageSprite;
	}
	else {
		m_iImageSprite = m_iMaxImageSprite;
		m_fCnt = 0;
	}
}

int Player::update(float fDeltaTile)
{
	Object::update(fDeltaTile);
	return 0;
}


void Player::render(HDC hdc)
{
	Object::render(hdc);
	RECT t = { m_tPosition.left, m_tPosition.top, m_tPosition.right, m_tPosition.bottom };

#ifdef DEBUG
	FrameRect(hdc,&t, (HBRUSH)GetStockObject(BLACK_BRUSH));
#endif // DEBUG


	m_tImg.TransparentBlt(hdc, t.left, t.top, 25, 50, m_iImageSprite * 57, 69 * (m_eMoveDir == MD_FRONT ? 0 : 1), 57, 69, RGB(80, 40, 0));

	//printf("m_eMoveDir : %d\n", m_eMoveDir);
}
