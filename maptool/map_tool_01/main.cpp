// 먼저 해야할 것
// 맵 사이즈 
//
//

#pragma warning (disable : 4996)
#include <Windows.h>
#include <tchar.h>
#include <atlimage.h>
#include <vector>
#include "resource1.h"

#ifdef _DEBUG
#include <iostream>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif // _DEBUG

#define TILESIZE 40

#define XLEN 100
#define YLEN 20

using namespace std;

LPCTSTR lpszWndName = TEXT("LETS MAKE MAP");
LPCTSTR lpszClassName = TEXT("Class Name");
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelessDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

enum MOVE_DIR {
	MD_BACK = -1,
	MD_NONE,
	MD_FRONT
};

enum STEP_FOR {
	SF_YELLOW = 0,
	SF_PUR,
	SF_ALL
};

enum TILE_DATA {
	TD_NON = 0,			// 아무것도 아님
	TD_BLOCK,			// 지나갈수 없음
	TD_FLOOR,			// 바닥
	TD_SPIKE			// 닿으면 죽음
};


struct Monster {
	RECT initpos;
	MOVE_DIR heading;
};

struct RollerCoaster {
	int group;
	RECT initpos;

	BOOL bActive;
	RECT moveTo;
	STEP_FOR type;
};

struct Step {
	int group;
	BOOL bActive;

	RECT initpos;
	STEP_FOR type;
};

struct Button {
	RECT initpos;
	int groupCtrl;
	BOOL bOnlie;
};




int g_iCurSet = 8;
bool g_bModalOpen = false;


enum CUR_SETTING {
	TILE = 0,
	MOVING_OBJ,
	STATIC_OBJ
};

//enum TILE_DATA {
//	TD_NON = 0,			// 아무것도 아님
//	TD_BLOCK,			// 지나갈수 없음
//	TD_FLOOR,			// 바닥
//	TD_SPIKE			// 닿으면 죽음
//};

enum MOVING_TYPE {
	MONSTER = 0,
	ROLLERCOASTER		// 미구현
};

enum STATIC_TYPE {
	STEP = 0,
	BUTTON				// 미구현
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	HWND hWnd;
	WNDCLASSEX WndClass;
	hInst = hInstance;

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.lpfnWndProc = WndProc;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClassName;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);


	RECT rt = { 0,0,1280,720 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, NULL);
	hWnd = CreateWindow(lpszClassName, lpszWndName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		rt.right - rt.left, rt.bottom - rt.top, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HBITMAP hBit;

	HBRUSH hBrush, oBrush;
	HANDLE hFile;
	// size (num of tiles)
	static SIZE sWndSize = { XLEN,YLEN };

	// tiles
	static vector<int> listTile;

	// moving type
	static vector<Monster*> listMonster;
	static vector<RollerCoaster*> listRollerCoaster;

	// static type
	static vector<Step*> listStep;
	static vector<Button*> listButton;

	static bool bDragging = false;
	static POINT ptStart;
	static POINT ptCurrent;

	static RECT curCam = { 0,0,1280,720 };
	static POINT psOffset;

	HWND hDlg = NULL;
	static int dx = 0, dy = 0;
	static POINT ptTotalOffset = { 0,0 };
	static RECT rtTemp;
	//static bool bStLeft = false;

	TCHAR str[30];

	switch (Msg) {
	case WM_CREATE:
	{
		// init
		for (int i = 0; i < sWndSize.cy; ++i) {
			for (int j = 0; j < sWndSize.cx; ++j) {
				listTile.push_back(TD_NON);
			}
		}
		// open dialog window(modeless) and set wndSize;
		g_bModalOpen = true;
		hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ModelessDlgProc);
		ShowWindow(hDlg, SW_SHOW);
		break;
	}


	case WM_KEYDOWN:
		if (wParam == VK_RETURN && !g_bModalOpen) {
			hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ModelessDlgProc);
			ShowWindow(hDlg, SW_SHOW);
		}
		if (wParam == VK_DELETE) {
			auto iter0 = listMonster.begin();
			for (iter0; iter0 != listMonster.end();) {
				if (PtInRect(&(*iter0)->initpos, ptCurrent)) iter0 = listMonster.erase(iter0);
				else iter0++;
			}
			auto iter1 = listRollerCoaster.begin();
			for (iter1; iter1 != listRollerCoaster.end();) {
				if (PtInRect(&(*iter1)->initpos, ptCurrent)) iter1 = listRollerCoaster.erase(iter1);
				else iter1++;
			}
			auto iter2 = listStep.begin();
			for (iter2; iter2 != listStep.end();) {
				if (PtInRect(&(*iter2)->initpos, ptCurrent)) iter2 = listStep.erase(iter2);
				else iter2++;
			}
			auto iter3 = listButton.begin();
			for (iter3; iter3 != listButton.end();) {
				if (PtInRect(&(*iter3)->initpos, ptCurrent)) iter3 = listButton.erase(iter3);
				else iter3++;
			}
		}
		if (wParam == VK_ESCAPE) {
			printf("Save File\n");
			printf("Save As (asdf.txt): ");
			char str[40];
			scanf("%s", str);

			FILE* fp = fopen(str, "w");

			// 타일맵의 크기
			fprintf(fp, "%d %d\n", sWndSize.cx, sWndSize.cy);
			// 타일맵 내용
			for (int i = 0; i < sWndSize.cy; ++i) {
				for (int j = 0; j < sWndSize.cx; ++j) {
					fprintf(fp, "%d ", listTile[i * sWndSize.cx + j]);
				}
				fprintf(fp, "\n");
			}

			// 몬스터
			// 몬스터의 길이와 몬스터 정보
			fprintf(fp, "%d\n", listMonster.size());
			for (auto const& d : listMonster) {
				RECT t = d->initpos;
				fprintf(fp, "%d %d %d %d %d\n", t.left, t.top, t.right, t.bottom, d->heading);
			}

			// 롤코
			// 롤코의 길이와 롤코 정보
			fprintf(fp, "%d\n", listRollerCoaster.size());
			for (auto const& d : listRollerCoaster) {
				// init Pos
				RECT t = d->initpos;
				fprintf(fp, "%d %d %d %d ", t.left, t.top, t.right, t.bottom);

				// type
				fprintf(fp, "%d ", d->type);

				// active
				fprintf(fp, "%d ", d->bActive);

				// group
				fprintf(fp, "%d ", d->group);

				// move Pos
				t = d->moveTo;
				fprintf(fp, "%d %d %d %d\n", t.left, t.top, t.right, t.bottom);
			}

			// 발판
			fprintf(fp, "%d\n", listStep.size());
			for (auto const& d : listStep) {
				// init Pos
				RECT t = d->initpos;
				fprintf(fp, "%d %d %d %d ", t.left, t.top, t.right, t.bottom);

				// type
				fprintf(fp, "%d ", d->type);

				// active
				fprintf(fp, "%d ", d->bActive);

				// group
				fprintf(fp, "%d\n", d->group);
			}

			// 버튼
			fprintf(fp, "%d\n", listButton.size());
			for (auto const& d : listButton) {
				// init Pos
				RECT t = d->initpos;
				fprintf(fp, "%d %d %d %d ", t.left, t.top, t.right, t.bottom);

				// fixed
				fprintf(fp, "%d ", d->bOnlie);

				// group
				fprintf(fp, "%d\n", d->groupCtrl);
			}
			fclose(fp);
		}
		if (wParam == VK_F1) {
			printf("Load File\n");
			printf("Type file name to load (asdf.txt): ");
			char str[40];
			scanf("%s", str);

			FILE* fp = fopen(str, "r");

			if (fp == NULL) {
				printf("invalid name!!\n");
				break;
			}

			listTile.clear();
			listButton.clear();
			listMonster.clear();
			listRollerCoaster.clear();
			listStep.clear();

			// get x,y size
			fscanf(fp, "%d %d", &sWndSize.cx, &sWndSize.cy);

			// get tiles
			for (int i = 0; i < sWndSize.cy; ++i) {
				for (int j = 0; j < sWndSize.cx; ++j) {
					TILE_DATA temp;
					fscanf(fp, "%d", &temp);

					/*Tile* t = new Tile(temp);*/

					listTile.push_back(temp);

				}
			}

			int size;
			// monster
			fscanf(fp, "%d", &size);
			for (int i = 0; i < size; ++i) {
				RECT t;
				MOVE_DIR md;
				fscanf(fp, "%d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &md);

				Monster* temp = new Monster;
				temp->initpos = t;
				temp->heading = md;
				//printf("%d %d %d %d\n", t.left, t.top, t.right, t.bottom);
				listMonster.push_back(temp);
			}

			// rollercoaster
			fscanf(fp, "%d", &size);
			for (int i = 0; i < size; ++i) {
				RECT t;
				STEP_FOR sf;
				BOOL b;
				int g;
				RECT mt;
				fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &sf, &b, &g, &mt.left, &mt.top, &mt.right, &mt.bottom);

				//RollerCoaster(RECT pos, STEP_FOR t, BOOL b, int g, RECT mt);
				RollerCoaster* temp = new RollerCoaster;
				temp->initpos = t;
				temp->type = sf;
				temp->bActive = b;
				temp->group = g;
				temp->moveTo = mt;

				listRollerCoaster.push_back(temp);
			}

			// step
			fscanf(fp, "%d", &size);
			for (int i = 0; i < size; ++i) {
				RECT t;
				STEP_FOR sf;
				int group;
				BOOL alive;
				fscanf(fp, "%d %d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &sf, &alive, &group);

				Step* temp = new Step;
				temp->initpos = t;
				temp->type = sf;
				temp->bActive = alive;
				temp->group = group;


				listStep.push_back(temp);
			}

			// button
			fscanf(fp, "%d", &size);
			for (int i = 0; i < size; ++i) {
				RECT t;
				BOOL fixed;
				int g;
				fscanf(fp, "%d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &fixed, &g);

				Button* temp = new Button;

				temp->initpos = t;
				temp->groupCtrl = g;
				temp->bOnlie = fixed;

				listButton.push_back(temp);
			}

			printf("file load complite\n");
			fclose(fp);
		}
		break;

	case WM_LBUTTONDOWN:
		bDragging = true;
		ptStart = { LOWORD(lParam), HIWORD(lParam) };

		ptCurrent = { LOWORD(lParam), HIWORD(lParam) };
		break;

	case WM_LBUTTONUP:
		if (bDragging) {
			bDragging = false;
			ptCurrent = { LOWORD(lParam), HIWORD(lParam) };

			switch (g_iCurSet) {
				// Tiles
				// change settings
			case IDC_RADIO1:
			case IDC_RADIO2:
			case IDC_RADIO9:
			case IDC_RADIO3:
				ptCurrent.x += ptTotalOffset.x;
				ptCurrent.y += ptTotalOffset.y;

				for (int i = (ptStart.y + ptTotalOffset.y) / TILESIZE; i <= ptCurrent.y / TILESIZE; ++i) {
					for (int j = (ptStart.x + ptTotalOffset.x) / TILESIZE; j <= ptCurrent.x / TILESIZE; ++j) {
						listTile[i * sWndSize.cx + j] = g_iCurSet - 1001;
					}
				}
				break;

				// Objs
				// push back Objs
			case IDC_RADIO4:	// Monster
			{
				RECT temp = { ptCurrent.x + ptTotalOffset.x - 15, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE - 30,
				ptCurrent.x + ptTotalOffset.x + 15, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE };

				Monster* ms = new Monster;

				ms->initpos = temp;
				int t;
				printf("Heading (-1,0,1) : ");
				scanf("%d", &t);
				if (t < 0) ms->heading = MD_BACK;
				else if (t > 0) ms->heading = MD_FRONT;
				else ms->heading = MD_NONE;

				listMonster.push_back(ms);
			}
			break;

			case IDC_RADIO5:	// Rollercoaster
			{
				RECT temp = { ptCurrent.x + ptTotalOffset.x - 25, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 25, ptCurrent.y + ptTotalOffset.y + 10 };

				RollerCoaster* rc = new RollerCoaster;

				rc->initpos = temp;

				int t;

				printf("YELLOW? PURPLE? ALL?\n");
				printf("0    1    2 : ");
				scanf("%d", &t);
				switch (t) {
				case 0:		rc->type = SF_YELLOW;	break;
				case 1:		rc->type = SF_PUR;		break;
				default:	rc->type = SF_ALL;		break;
				}

				printf("is Active");
				scanf("%d", &t);
				rc->bActive = t;

				printf("which group : ");
				scanf("%d", &t);
				rc->group = t;//

				printf("move x : ");
				scanf("%d", &t);
				OffsetRect(&temp, t, 0);
				printf("move y : ");
				scanf("%d", &t);
				OffsetRect(&temp, 0, t);

				rc->moveTo = temp;
				
				listRollerCoaster.push_back(rc);
			}
				break;


			case IDC_RADIO6:	// step
			{
				RECT temp = { ptCurrent.x + ptTotalOffset.x - 25, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 25, ptCurrent.y + ptTotalOffset.y + 10 };

				Step* st = new Step;
				st->initpos = temp;
				int t;
				printf("YELLOW? PURPLE? ALL?\n");
				printf("0    1    2 : ");
				scanf("%d", &t);
				switch (t) {
				case 0:		st->type = SF_YELLOW;	break;
				case 1:		st->type = SF_PUR;		break;
				default:	st->type = SF_ALL;		break;
				}
				printf("which group(-1 : no group) : ");
				scanf("%d", &t);
				st->group = t;//

				printf("alive? (0 : false 1 : true) :");
				scanf("%d", &t);
				st->bActive = t;

				listStep.push_back(st);
			}
				break;
			case IDC_RADIO7:	// button
			{
				RECT temp = { ptCurrent.x + ptTotalOffset.x - 10, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE - 20,
					ptCurrent.x + ptTotalOffset.x + 10, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE };
				int t;

				
				Button* bt = new Button;
				bt->bOnlie = false;
				bt->initpos = temp;

				printf("ctrl group : ");
				scanf("%d", &t);
				bt->groupCtrl = t;
				listButton.push_back(bt);
			}
				break;

			case IDC_RADIO8:
				ptTotalOffset.x += dx;
				ptTotalOffset.y += dy;
				dx = 0, dy = 0;
				break;
			}

			if (g_iCurSet >= IDC_RADIO4) g_iCurSet = IDC_RADIO8;

		}


		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEMOVE:
		ptCurrent = { LOWORD(lParam), HIWORD(lParam) };
		switch (g_iCurSet) {
			// Tiles
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO9:
		case IDC_RADIO3:
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ptCurrent = { LOWORD(lParam), HIWORD(lParam) };
			break;

			// Objs
		case IDC_RADIO4:
		case IDC_RADIO5:
		case IDC_RADIO6:
		case IDC_RADIO7:
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			ptCurrent = { LOWORD(lParam), HIWORD(lParam) };
			break;

			// Look Around;
		case IDC_RADIO8:
			SetCursor(LoadCursor(NULL, IDC_HAND));
			ptCurrent = { LOWORD(lParam), HIWORD(lParam) };

			if (bDragging) {

				dx = ptStart.x - ptCurrent.x;
				dy = ptStart.y - ptCurrent.y;

				if (ptTotalOffset.x + dx < 0) dx = -ptTotalOffset.x;
				else if (ptTotalOffset.x + dx > sWndSize.cx * TILESIZE - 1280) dx = sWndSize.cx * TILESIZE - ptTotalOffset.x - 1280;
				if (ptTotalOffset.y + dy < 0) dy = -ptTotalOffset.y;
				else if (ptTotalOffset.y + dy > sWndSize.cy * TILESIZE - 720)
					dy = sWndSize.cy * TILESIZE - ptTotalOffset.y - 720;


			}
			break;

		default:
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;


		// print cur pos
	case WM_RBUTTONDOWN:
	{
		int x = ptTotalOffset.x;
		int y = ptTotalOffset.y;

		x += LOWORD(lParam);
		y += HIWORD(lParam);

		x /= TILESIZE;
		y /= TILESIZE;

		printf("(%d, %d)\n", x, y);
	}
	break;


	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		hBit = CreateCompatibleBitmap(hdc, TILESIZE * sWndSize.cx, TILESIZE * sWndSize.cy);
		SelectObject(memdc, hBit);

		// 타일 채우기
		for (int y = 0; y < sWndSize.cy; ++y) {
			for (int x = 0; x < sWndSize.cx; ++x) {
				switch (listTile[y * sWndSize.cx + x]) {
				case TD_NON:
					Rectangle(memdc, x * TILESIZE, y * TILESIZE, (x + 1) * TILESIZE, (y + 1) * TILESIZE);
					break;

				case TD_BLOCK:
				{
					hBrush = CreateSolidBrush(RGB(255, 0, 0));
					RECT temp = { x * TILESIZE, y * TILESIZE, (x + 1) * TILESIZE, (y + 1) * TILESIZE };
					FillRect(memdc, &temp, hBrush);
					FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
					DeleteObject(hBrush);
				}
				break;

				case TD_FLOOR:
				{
					hBrush = CreateSolidBrush(RGB(125, 0, 0));
					RECT temp = { x * TILESIZE, y * TILESIZE, (x + 1) * TILESIZE, (y + 1) * TILESIZE };
					FillRect(memdc, &temp, hBrush);
					FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
					DeleteObject(hBrush);
				}
				break;

				case TD_SPIKE:
				{
					hBrush = CreateSolidBrush(RGB(125, 125, 125));
					//Rectangle(memdc, x * TILESIZE, y * TILESIZE, (x + 1) * TILESIZE, (y + 1) * TILESIZE);
					RECT temp = { x * TILESIZE, y * TILESIZE, (x + 1) * TILESIZE, (y + 1) * TILESIZE };
					FillRect(memdc, &temp, hBrush);
					FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
					DeleteObject(hBrush);
				}
				break;
				}

			}
		}


		// 몬스터 그려놓기
		for (const auto& d : listMonster) {
			RECT t = d->initpos;
			Rectangle(memdc, t.left, t.top, t.right, t.bottom);
			
			switch (d->heading) {
			case MD_BACK:		TextOut(memdc, t.left + 5, t.top + 1, L"BAK", 3);		break;
			case MD_FRONT:		TextOut(memdc, t.left + 5, t.top + 1, L"FOR", 3);		break;
			case MD_NONE:		TextOut(memdc, t.left + 5, t.top + 1, L"STP", 3);		break;
			default:			TextOut(memdc, t.left + 5, t.top + 1, L"DEF", 3);		break;
			}
		}
		// 롤코 그려놓기
		for (const auto& d : listRollerCoaster) {
			RECT t = d->initpos;
			Rectangle(memdc, t.left, t.top, t.right, t.bottom);

			wsprintf(str, L"%d %d", d->group, d->type);
			TextOut(memdc, t.left + 5, t.top + 13, str, _tcsclen(str));
		}


		// 발판 그려놓기
		for (const auto& d : listStep) {
			RECT t = d->initpos;
			Rectangle(memdc, t.left, t.top, t.right, t.bottom);
			switch (d->type) {
			case SF_ALL:		TextOut(memdc, t.left + 5, t.top + 1, L"ALL", 3);		break;
			case SF_YELLOW:		TextOut(memdc, t.left + 5, t.top + 1, L"YEL", 3);		break;
			case SF_PUR:		TextOut(memdc, t.left + 5, t.top + 1, L"PUR", 3);		break;
			default:			TextOut(memdc, t.left + 5, t.top + 1, L"DEF", 3);		break;
			}

			wsprintf(str, L"%d %d", d->bActive,  d->group);
			TextOut(memdc, t.left + 5, t.top + 13, str, _tcsclen(str));
		}
		// 버튼 그려놓기
		for (const auto& d : listButton) {
			RECT t = d->initpos;
			Rectangle(memdc, t.left, t.top, t.right, t.bottom);

			
			wsprintf(str, L"%d, %d", d->groupCtrl);
			TextOut(memdc, t.left + 5, t.top + 13, str, _tcsclen(str));
		}


		// 드래그
		if (bDragging && g_iCurSet <= IDC_RADIO3) {
			RECT temp = { ptStart.x, ptStart.y, ptCurrent.x, ptCurrent.y };
			OffsetRect(&temp, ptTotalOffset.x, ptTotalOffset.y);
			FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}


		// 커서 보여주기
		switch (g_iCurSet) {
			// tiles
		case IDC_RADIO1:		// non
			Rectangle(memdc, ptCurrent.x + ptTotalOffset.x - 10, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 10, ptCurrent.y + ptTotalOffset.y + 10);
			break;
		case IDC_RADIO2:		// block
		{
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			RECT temp = { ptCurrent.x + ptTotalOffset.x - 10, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 10, ptCurrent.y + ptTotalOffset.y + 10 };
			FillRect(memdc, &temp, hBrush);
			FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
			DeleteObject(hBrush);
		}
			break;
		case IDC_RADIO9:		// floor
		{
			hBrush = CreateSolidBrush(RGB(125, 0, 0));
			RECT temp = { ptCurrent.x + ptTotalOffset.x - 10, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 10, ptCurrent.y + ptTotalOffset.y + 10 };
			FillRect(memdc, &temp, hBrush);
			FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
			DeleteObject(hBrush);
		}
			break;
		case IDC_RADIO3:		// spike
		{
			hBrush = CreateSolidBrush(RGB(125, 125, 125));
			RECT temp = { ptCurrent.x + ptTotalOffset.x - 10, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 10, ptCurrent.y + ptTotalOffset.y + 10 };
			FillRect(memdc, &temp, hBrush);
			FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
			DeleteObject(hBrush);
		}
			break;

			// Objs
		case IDC_RADIO4:		// monster
			Rectangle(memdc, ptCurrent.x + ptTotalOffset.x - 15, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE - 30,
				ptCurrent.x + ptTotalOffset.x + 15, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE);
			break;
		case IDC_RADIO5:		// rollercoaster
			Rectangle(memdc, ptCurrent.x + ptTotalOffset.x - 25, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 25, ptCurrent.y + ptTotalOffset.y + 10);
			break;

		case IDC_RADIO6:		// step
			Rectangle(memdc, ptCurrent.x + ptTotalOffset.x - 25, ptCurrent.y + ptTotalOffset.y - 10,
				ptCurrent.x + ptTotalOffset.x + 25, ptCurrent.y + ptTotalOffset.y + 10);
			break;
		case IDC_RADIO7:		// button
			Rectangle(memdc, ptCurrent.x + ptTotalOffset.x - 10, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE - 20,
				ptCurrent.x + ptTotalOffset.x + 10, (ptCurrent.y + ptTotalOffset.y) / TILESIZE * TILESIZE + TILESIZE);
			break;
		}


		//StretchBlt(hdc, 0, 0, 1280, 720, memdc, curCam.left + dx, curCam.top + dy, curCam.right + dx, curCam.bottom + dy, SRCCOPY);
		BitBlt(hdc, 0, 0, 1280, 720, memdc, ptTotalOffset.x + dx, ptTotalOffset.y + dy, SRCCOPY);


		DeleteObject(hBit);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}


	return DefWindowProc(hWnd, Msg, wParam, lParam);
}


BOOL CALLBACK ModelessDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			g_bModalOpen = false;
			DestroyWindow(hWnd);
			hWnd = NULL;
			break;

		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO9:
		case IDC_RADIO3:
		case IDC_RADIO4:
		case IDC_RADIO5:
		case IDC_RADIO6:
		case IDC_RADIO7:
		case IDC_RADIO8:
			g_iCurSet = LOWORD(wParam);
			printf("cur Sel : %d\n", g_iCurSet - 1001);
			break;
		}
		break;
		

	default:
		//return 1;
		break;
	}
	return 0;
}