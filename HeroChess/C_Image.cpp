#include "stdafx.h"
#include "C_Image.h"

void C_Image::Render(HDC memdc)
{
    //더블 버퍼링
//메시지가 발생할 때마다 DC가 초기화하므로
//교체하는 방식이 아닌 그리는 DC와 화면에 표시하는 DC를 따로 정해두는 게 좋다.

    HDC image_dc = CreateCompatibleDC(memdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, BackgroundBit);
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    StretchBlt(memdc, 0, 0, WIN_W, WIN_H, image_dc, 0, 0, IMAGE_W, IMAGE_H, SRCCOPY);

    //DC 파괴하기
    SelectObject(image_dc, oldBit);
    DeleteDC(image_dc);
}