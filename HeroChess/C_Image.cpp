#include "stdafx.h"
#include "C_Image.h"

void C_Image::Render(HDC memdc)
{
    //���� ���۸�
//�޽����� �߻��� ������ DC�� �ʱ�ȭ�ϹǷ�
//��ü�ϴ� ����� �ƴ� �׸��� DC�� ȭ�鿡 ǥ���ϴ� DC�� ���� ���صδ� �� ����.

    HDC image_dc = CreateCompatibleDC(memdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, BackgroundBit);
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    StretchBlt(memdc, 0, 0, WIN_W, WIN_H, image_dc, 0, 0, IMAGE_W, IMAGE_H, SRCCOPY);

    //DC �ı��ϱ�
    SelectObject(image_dc, oldBit);
    DeleteDC(image_dc);
}