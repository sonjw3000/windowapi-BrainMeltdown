#include "Core.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	if (!Core::GetInst()->Init(hInstance)) {
		Core::DestroyInst();
		return 0;
	}

	RECT temp;

	int retValue = Core::GetInst()->Run();

	Core::DestroyInst();

	return retValue;
}