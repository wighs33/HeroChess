#include "stdafx.h"
#include "C_Image.h"

void C_Image::Render(HDC hdc)
{
    memdc = CreateCompatibleDC(hdc);
    oldBit = (HBITMAP)SelectObject(memdc, BackgroundBit);
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    StretchBlt(hdc, 0, 0, WIN_W - 10, WIN_H - 50, memdc, 0, 0, IMAGE_W, IMAGE_H, SRCCOPY);

    //DC �ı��ϱ�
    SelectObject(memdc, oldBit);
    DeleteDC(memdc);
}