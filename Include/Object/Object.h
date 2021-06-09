#pragma once

#include "../Game.h"

class Object {
protected:
	Object();

public:
	virtual ~Object();

protected:
	string m_strTag = "";
	FRECT m_tPosition = { 0,0,0,0 };
	CImage m_tImg;

public:
	void setTag(string const& strTag) { m_strTag = strTag; }
	void setPosition(RECT const& tPos) { m_tPosition = tPos; }

	string getTag() const { return m_strTag; }
	FRECT getPosition() const { return m_tPosition; }

public:
	virtual bool init() = 0;

	virtual void input(float fDeltaTile);
	virtual int update(float fDeltaTile);
	virtual void render(HDC hdc);

public:
	//template <typename T>
	//static T* CreateObj(string const& strTag) {
	//	T* pObj = new T;

	//	if (!pObj->Init()) {
	//		SAFE_DELETE(pObj);
	//		return NULL;
	//	}
	//	return pObj;
	//}

};