#include "Object.h"


Object::Object()
{
}

Object::~Object()
{
	if ((HBITMAP)m_tImg != NULL) m_tImg.Destroy();
}


void Object::input(float fDeltaTime)
{
}

int Object::update(float fDeltaTime)
{
	return 0;
}


void Object::render(HDC hdc)
{
}