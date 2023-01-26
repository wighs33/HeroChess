// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <memory>
#include <random>
#include <vector>

//임시
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

static const int WHITE = RGB(255, 255, 255);
static const int BLUE = RGB(0, 0, 255);
static const int GREEN = RGB(0, 255, 0);
static const int YELLOW = RGB(255, 230, 0);
static const int RED = RGB(255, 0, 0);
static const int GRAY = RGB(100, 100, 100);
static const int PURPLE = RGB(147, 112, 219);
static const int WIN_W = 560;
static const int WIN_H = 780;
static const int GRID_WH = 60;
static const int BOARD_W = 7;
static const int BOARD_H = 10;

// 전역 변수:
static HINSTANCE hInst;                                // 현재 인스턴스입니다.
static WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
static WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

static random_device seeder;
static const auto seed = seeder.entropy() ? seeder() : time(nullptr);
static mt19937 eng(static_cast<mt19937::result_type>(seed));
static uniform_int_distribution dist(3, BOARD_H - 4);

enum HERO
{
	MAGICIAN = 0,
	REAPER,
	NINJA,
	GHOST,
	WARRIOR,
	DEFENDER,
	KNIGHT
};

enum WHO
{
	PLAYER1 = 1,
	PLAYER2,
	ALL
};