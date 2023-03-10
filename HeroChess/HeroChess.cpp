// HeroChess.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "HeroChess.h"
#include "Resource.h"
#include "C_Hero.h"
#include "C_Command.h"
#include "C_Image.h"
#include "C_InputHandler.h"
#include "C_Board.h"

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
    //AllocConsole();
    //freopen("CONOUT$", "wt", stdout);

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
    while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    return msg.wParam;
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
    //메인 DC, 임시 DC
    HDC hdc, memdc;
    static HBITMAP hBit, oldBit;


	PAINTSTRUCT ps;

    //static C_Magician magician(true, 100, 300);
    static C_Image background(IDB_BITMAP1, 320, 320);
    static C_Image castle(IDB_BITMAP50, 674, 867);
    static C_Image fire(IDB_BITMAP51, 260, 260);
    static C_InputHandler input;
    static C_Board board;

    switch (message)
    {
    case WM_CREATE:
        input.Bind_Command();

        SetTimer(hWnd, 1, 70, NULL);

        break;

    // 타이머 있을 때 반복 처리
    case WM_TIMER:
        //메인 도화지 가져오기
        hdc = GetDC(hWnd);

        //hBit를 hdc와 호환되게 만들어준다: 여기에 이미지를 모아서 저장하려고 함 
        if (!hBit)
            hBit = CreateCompatibleBitmap(hdc, WIN_W, WIN_H);

        //임시 도화지 생성
        memdc = CreateCompatibleDC(hdc);
        oldBit = (HBITMAP)SelectObject(memdc, hBit);

        background.Render(memdc, 0, 0, WIN_W, WIN_H);
        board.Render(memdc);
        board.Act_Hero();
        board.Render_Heroes(memdc);
        castle.Render(memdc, board.Index_To_Pos(3), board.Index_To_Pos(0)- GRID_WH/2, GRID_WH, GRID_WH + GRID_WH / 2);
        castle.Render(memdc, board.Index_To_Pos(3), board.Index_To_Pos(BOARD_H - 1)- GRID_WH/2, GRID_WH, GRID_WH + GRID_WH / 2);

        //성이 불타는 이미지
        if(board.Lifes().first <= 1)
            fire.Render(memdc, board.Index_To_Pos(3), board.Index_To_Pos(0) - GRID_WH / 2, GRID_WH, GRID_WH + GRID_WH / 2);
        if (board.Lifes().second <= 1)
            fire.Render(memdc, board.Index_To_Pos(3), board.Index_To_Pos(BOARD_H - 1) - GRID_WH / 2, GRID_WH, GRID_WH + GRID_WH / 2);

        if (board.Lifes().first == 0 || board.Lifes().second == 0)
        {
            KillTimer(hWnd, 1);
            static TCHAR lpOut[100];
            wsprintf(lpOut, _T("Player %d 승리!"), (board.Lifes().first == 0 ? 2 : 1));

            //차일드 윈도우 띄우기
            MessageBox(hWnd, lpOut, _T("승자"), MB_OKCANCEL);
            break;
        }

        //비트맵 지우기
        SelectObject(memdc, oldBit);

        //임시 도화지 삭제
        DeleteDC(memdc);

        ReleaseDC(hWnd, hdc);
        InvalidateRgn(hWnd, NULL, false);
        break;

    case WM_PAINT:
        //메인 도화지 생성
        hdc = BeginPaint(hWnd, &ps);

        memdc = CreateCompatibleDC(hdc);
        oldBit = (HBITMAP)SelectObject(memdc, hBit);
        //임시 도화지의 그림내용 메인 도화지로 옮기기
        BitBlt(hdc, 0, 0, WIN_W, WIN_H, memdc, 0, 0, SRCCOPY);

        SelectObject(memdc, oldBit);
        DeleteDC(memdc);

        //메인 도화지 삭제
        EndPaint(hWnd, &ps);
        break;
    case WM_LBUTTONDOWN:
        board.Check_Click(LOWORD(lParam), HIWORD(lParam));
        InvalidateRgn(hWnd, NULL, false);
        break;
    case WM_COMMAND:
    {
        // 메뉴 선택을 구문 분석합니다:
        switch (LOWORD(wParam))
        {
        case ID_NEWGAME:
        {
            int answer = MessageBox(hWnd, _T("게임을 새로 시작하겠습니까?"), _T("새 게임"), MB_OKCANCEL);
            if (answer == IDOK)
            {
                C_Board newboard;
                board = newboard;
            }
        }
        break;
        case ID_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        if (hBit) DeleteObject(hBit);
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}