#include "stdafx.h"
#include "C_Image.h"

void C_Image::Render(HDC memdc, int sx, int sy, int ex, int ey)
{
    //���� ���۸�
//�޽����� �߻��� ������ DC�� �ʱ�ȭ�ϹǷ�
//��ü�ϴ� ����� �ƴ� �׸��� DC�� ȭ�鿡 ǥ���ϴ� DC�� ���� ���صδ� �� ����.

    HDC image_dc = CreateCompatibleDC(memdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, BackgroundBit);
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, sx, sy, ex, ey, image_dc, 0, 0, image_w, image_h, WHITE);

    //DC �ı��ϱ�
    SelectObject(image_dc, oldBit);
    DeleteDC(image_dc);
}