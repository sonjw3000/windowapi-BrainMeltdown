#pragma once
#pragma warning (disable : 4996)
#include <Windows.h>
#include <vector>
#include <list>
#include <string>
#include <atlimage.h>
#include <fmod.h>

#ifdef _DEBUG
#include <crtdbg.h>
#include <iostream>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif // _DEBUG


using namespace std;

#include "Type.h"
#include "Macro.h"

#define TILESIZE 40


template <typename T>
void Safe_Delete_VecList(T& p) {
	typename T::iterator iter;
	for (iter = p.begin(); iter != p.end(); ++iter) SAFE_DELETE((*iter));

	p.clear();
}