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
		m_tImg.Load(L"Resource/player1.png");
	}
	else {
		setPosition({ 175,150,200,200 });
		m_tImg.Load(L"Resource/player2.bmp");

	}

	m_iSpriteY = m_eMoveDir == MOVE_DIR::MD_BACK;
	return true;
}

void Player::input(float fDeltaTime)
{
	m_tBefPos = m_tPosition;

	Object::input(fDeltaTime);
	//if (m_bFalling) printf("jumping\n");

	bool bMoved = false;
	m_bSit = false;

	// input
	if (m_iPlayerNum == 0) {
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			Move(-getSpeed() * fDeltaTime / (m_bSitComplete + 1), 0);
			m_eMoveDir = MOVE_DIR::MD_BACK;
			bMoved = true;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			Move(getSpeed() * fDeltaTime / (m_bSitComplete + 1), 0);
			m_eMoveDir = MOVE_DIR::MD_FRONT;
			bMoved = true;
		}
		if (!m_bFalling) {
			if (GetAsyncKeyState(VK_UP) & 0x8000) {
				m_bFalling = true;
				m_fJumpSpeed = -500;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !m_bLand) {
				m_bSit = true;
			}
			else {
				m_bSitComplete = false;
			}
		}
		//if (GetAsyncKeyState(VK_DOWN)) Move(0, getSpeed());
	}
	else {
		if (GetAsyncKeyState('A') & 0x8000) {
			Move(-getSpeed() * fDeltaTime / (m_bSitComplete + 1), 0);
			m_eMoveDir = MOVE_DIR::MD_BACK;
			bMoved = true;
		}
		if (GetAsyncKeyState('D') & 0x8000) {
			Move(getSpeed() * fDeltaTime / (m_bSitComplete + 1), 0);
			m_eMoveDir = MOVE_DIR::MD_FRONT;
			bMoved = true;
		}
		if (!m_bFalling) {
			if (GetAsyncKeyState('W') & 0x8000) {
				m_bFalling = true;
				m_fJumpSpeed = -500;
			}
			if (GetAsyncKeyState('S') & 0x8000 && !m_bLand) {
				m_bSit = true;
			}
			else m_bSitComplete = false;
		}
	}

	// setSprite
	m_iSpriteY = m_eMoveDir == MOVE_DIR::MD_BACK;


	// go down and set sprite
	if (m_bFalling) {
		Move(0.0, m_fJumpSpeed * fDeltaTime);

		m_fJumpSpeed += 500 * fDeltaTime * 3;
		if (m_fJumpSpeed >= 500) m_fJumpSpeed = 500;
		m_bBefJump = true;
	}

	// begin sit and set sprite;
	if (m_bFalling || m_bLand) {
		m_iSpriteY += 2;
	}
	else if (m_bSit) {
		m_iSpriteY += 4;
		if (m_bSitComplete) m_iSpriteY += 2;
	}
	if (m_bBefJump && !m_bFalling) {
		m_fCnt = 0;
		m_iImageSprite = 9;
		m_bLand = true;
		m_bBefJump = false;
	}

	if (m_bSit && !m_bSitComplete) {
		m_iImageSprite *= m_bBeforeSit;
		m_bBeforeSit = true;
		m_fCnt += fDeltaTime;
		if (m_fCnt >= 0.025) {
			m_iImageSprite++;
			m_fCnt = 0;
		}

		if (m_iImageSprite > 9 - m_iPlayerNum) {
			m_bSitComplete = true;
			m_iImageSprite = 5 - m_iPlayerNum;
			m_iSpriteY += 2;
		}
	}
	else if (m_bFalling) {
		// 공중에서 점프중
		if (m_iImageSprite < 0) m_iImageSprite = (m_fJumpSpeed / 300) + 3;
		else m_iImageSprite = static_cast<int>(m_fJumpSpeed / 150) + 3;

		m_bBefJump = true;
	}
	else if (m_bLand) {
		// 착지
		m_fCnt += fDeltaTime;
		if (m_fCnt >= (0.025 + 0.025 * m_iPlayerNum)) {
			m_iImageSprite++;
			m_fCnt = 0;
		}
		
		if (m_iImageSprite > 15 - (3 * m_iPlayerNum)) {
			m_bLand = false;
			m_iSpriteY -= 2;
			m_iImageSprite = 4;
		}
	}
	else if (bMoved) {
		m_fCnt += fDeltaTime;

		if (m_fCnt >= 0.075) {
			m_iImageSprite++;
			m_fCnt = 0;
		}
		m_iImageSprite %= m_iMaxImageSprite + m_bSitComplete - m_iPlayerNum;
		
		m_bBeforeSit = false;
	}
	else {
		m_iImageSprite = m_iMaxImageSprite + m_bSitComplete - m_iPlayerNum;
		m_fCnt = 0;

		m_bBeforeSit = false;
	}



}

int Player::update(float fDeltaTime)
{
	Object::update(fDeltaTime);
	return 0;
}


void Player::render(HDC hdc)
{
	Object::render(hdc);
	RECT t = { m_tPosition.left, m_tPosition.top, m_tPosition.right, m_tPosition.bottom };

#ifdef DEBUG
	FrameRect(hdc,&t, (HBRUSH)GetStockObject(BLACK_BRUSH));
#endif // DEBUG


	m_tImg.TransparentBlt(hdc, t.left - 13, t.top, 50, 50, 
		m_iImageSprite * 72, 72 * m_iSpriteY, 70, 70, RGB(80, 40, 0));


	//printf("%d %d\n", m_iImageSprite, m_iSpriteY);
	//printf("m_eMoveDir : %d\n", m_eMoveDir);
}
