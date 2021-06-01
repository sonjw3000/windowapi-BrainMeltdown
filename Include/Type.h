#pragma once

#include <Windows.h>

// 실수형 사각형
typedef struct MY_FLOAT_RECT {
	float left, top, right, bottom;

	// operator overloading
	MY_FLOAT_RECT operator+(MY_FLOAT_RECT other) const {
		MY_FLOAT_RECT temp;
		temp.left += other.left;
		temp.top += other.top;
		temp.right += other.right;
		temp.bottom += other.bottom;

		return temp;
	}
	MY_FLOAT_RECT operator-(MY_FLOAT_RECT other) const {
		MY_FLOAT_RECT temp;
		temp.left -= other.left;
		temp.top -= other.top;
		temp.right -= other.right;
		temp.bottom -= other.bottom;

		return temp;
	}

	void operator=(MY_FLOAT_RECT other) {
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}
	void operator=(RECT other) {
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}


	// function
	BOOL IntersectRect(MY_FLOAT_RECT other) const {
		if (left < other.right &&
			top < other.bottom &&
			right > other.left &&
			bottom > other.top)		return TRUE;

		return FALSE;
	}
	BOOL IntersectRect(RECT other) const {
		MY_FLOAT_RECT temp;

		temp.operator=(other);
		return temp.IntersectRect(*this);
	}

	void OffsetRect(float dx, float dy) {
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}
	void OffsetRect(int dx, int dy) {
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}

}FRECT;

enum MOVE_DIR {
	MD_BACK = -1,
	MD_NONE,
	MD_FRONT
};

enum TILE_DATA {
	TD_NON = 0,			// 아무것도 아님
	TD_BLOCK,			// 지나갈수 없음
	TD_FLOOR,			// 바닥
	TD_SPIKE			// 닿으면 죽음
};