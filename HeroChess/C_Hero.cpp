#include "stdafx.h"
#include "C_Hero.h"

void C_Hero::Move_Per_Frame(int dest_x, int dest_y)
{
    if (x_ == dest_x and y_ == dest_y) move_ = 2;
    if (move_ == 2) return;

    int dist_x = dest_x - x_;
    int dist_y = dest_y - y_;

    cout << x_ << ' ' << y_ << endl;

    if (dist_x >= -5 and dist_x <= 5)
        x_ = dest_x;
    else
    {
        int move_x = 10 * dist_x / (int)sqrt(dist_x * dist_x + dist_y * dist_y);
        x_ += move_x;
    }

    if (dist_y >= -5 and dist_y <= 5)
        y_ = dest_y;
    else
    {
        int move_y = 10 * dist_y / (int)sqrt(dist_x * dist_x + dist_y * dist_y);
        y_ += move_y;
    }
}

void C_Magician::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % (N_IMAGES - 1);
    else
        count = 0;

    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, MagicianBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, MAGICIAN_W, MAGICIAN_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}