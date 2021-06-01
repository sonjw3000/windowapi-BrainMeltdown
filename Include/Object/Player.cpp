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
	setSpeed(1.5);

	if (m_iPlayerNum == 0) {
		setPosition({ 375,350,400,400 });
		m_tImg.Load(L"Resource/p0temp.png");
	}
	else {
		setPosition({ 175,150,200,200 });
		m_tImg.Load(L"Resource/p1temp.png");

	}


	return true;
}

void Player::input()
{
	m_tBefPos = m_tPosition;

	Object::input();
	if (m_iPlayerNum == 0) {
		if (GetAsyncKeyState(VK_LEFT)) Move(-getSpeed(), 0);
		if (GetAsyncKeyState(VK_RIGHT)) Move(getSpeed(), 0);
		if (!m_bFalling) if (GetAsyncKeyState(VK_UP)) {
			m_bFalling = true;
			m_bJumpSpeed = -10.0;
		}

		//if (GetAsyncKeyState(VK_DOWN)) Move(0, getSpeed());
	}
	else {
		if (GetAsyncKeyState('A')) Move(-getSpeed(), 0);
		if (GetAsyncKeyState('D')) Move(getSpeed(), 0);
		if (!m_bFalling) if (GetAsyncKeyState('W')) {
			m_bFalling = true;
			m_bJumpSpeed = -10.0;
		}
	}

	if (m_bFalling){
		Move(0.0, m_bJumpSpeed);

		m_bJumpSpeed += 0.5;
		if (m_bJumpSpeed >= 10) m_bJumpSpeed = 10;
	}

	//if (m_bFalling) jump();
}

int Player::update()
{
	Object::update();
	return 0;
}


void Player::render(HDC hdc)
{
	Object::render(hdc);
	RECT t = { m_tPosition.left, m_tPosition.top, m_tPosition.right, m_tPosition.bottom };
	//Rectangle(hdc, m_tPosition.left, m_tPosition.top, m_tPosition.right, m_tPosition.bottom);
	m_tImg.TransparentBlt(hdc, t, RGB(255,255,255));
}
