#include "Object.h"


Object::Object()
{
}

Object::~Object()
{
	if ((HBITMAP)m_tImg != NULL) m_tImg.Destroy();
}


void Object::input(float fDeltaTile)
{
}

int Object::update(float fDeltaTile)
{
	return 0;
}


void Object::render(HDC hdc)
{
}