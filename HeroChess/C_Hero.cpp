#include "stdafx.h"
#include "C_Hero.h"

void C_Magician::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    count = ++count % 16;
    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, MagicianBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x, y, MAGICIAN_W, MAGICIAN_H, image_dc, 0, 0, MAGICIAN_W, MAGICIAN_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}

void C_Magician::Move_Per_Frame(int dest_x, int dest_y)
{
    int dist_x = dest_x - x;
    int dist_y = dest_y - y;

    if (dist_x >= -5 and dist_x <= 5)
        x = dest_x;
    else
    {
        int move_x = 10 * dist_x / (int)sqrt(dist_x * dist_x + dist_y * dist_y);
        x += move_x;
    }

    if (dist_y >= -5 and dist_y <= 5)
        y = dest_y;
    else
    {
        int move_y = 10 * dist_y / (int)sqrt(dist_x * dist_x + dist_y * dist_y);
        y += move_y;
    }
}
