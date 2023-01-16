﻿// HeroChess.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "HeroChess.h"
#include "Resource.h"
#include "C_Hero.h"
#include "C_Command.h"
#include "C_Image.h"
#include "C_InputHandler.h"

const int START_W = 250;
const int START_H = 100;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    //디버깅용 : 콘솔 띄우기
    AllocConsole();
    freopen("CONOUT$", "wt", stdout);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HEROCHESS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HEROCHESS));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HEROCHESS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.


   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       START_W, START_H, WIN_W, WIN_H, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
	PAINTSTRUCT ps;

    static C_Magician magician;
    static C_Image background(IDB_BITMAP1);
    static C_InputHandler input;

    switch (message)
    {
    case WM_CREATE:
        input.Bind_Command();
        break;
    case WM_TIMER:
        //magician.Move_Per_Frame();
        input.Handle_Input(&magician);
        InvalidateRgn(hWnd, NULL, true);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		background.Set_HDC(hdc);
		magician.Set_HDC(hdc);

		background.Load_Image();
		magician.Load_Image();

		background.Render();
		magician.Render();

		background.Release_Image();
		magician.Release_Image();
		EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN:
        input.Set_Pressed(wParam);
        KillTimer(hWnd, 1);
        SetTimer(hWnd, 1, 100, NULL);
        break;
    case WM_KEYUP:
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

    //case WM_COMMAND:
    //{
    //    int wmId = LOWORD(wParam);
    //    // 메뉴 선택을 구문 분석합니다:
    //    switch (wmId)
    //    {
    //    case IDM_ABOUT:
    //        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
    //        break;
    //    case IDM_EXIT:
    //        DestroyWindow(hWnd);
    //        break;
    //    default:
    //        return DefWindowProc(hWnd, message, wParam, lParam);
    //    }
    //}
    //break;