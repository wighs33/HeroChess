#include "stdafx.h"
#include "C_Image.h"

void C_Image::Render(HDC hdc)
{
    memdc = CreateCompatibleDC(hdc);
    oldBit = (HBITMAP)SelectObject(memdc, BackgroundBit);
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    StretchBlt(hdc, 0, 0, WIN_W - 10, WIN_H - 50, memdc, 0, 0, IMAGE_W, IMAGE_H, SRCCOPY);

    //DC 파괴하기
    SelectObject(memdc, oldBit);
    DeleteDC(memdc);
}