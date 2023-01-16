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
#define MAX_LOADSTRING 100

static const int WHITE = RGB(255, 255, 255);
static const int WIN_W = 560;
static const int WIN_H = 780;

// 전역 변수:
static HINSTANCE hInst;                                // 현재 인스턴스입니다.
static WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
static WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.