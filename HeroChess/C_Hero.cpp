#include "stdafx.h"
#include "C_Hero.h"

void C_Magician::Load_Image()
{
    count = ++count % 16;
    for (int i = 0; i < 17; i++)
        MagicianBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2 + i));
}

void C_Magician::Render()
{
    memdc = CreateCompatibleDC(hdc_);
    oldBit = (HBITMAP)SelectObject(memdc, MagicianBit[count]);
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(hdc_, x, y, MAGICIAN_W, MAGICIAN_H, memdc, 0, 0, MAGICIAN_W, MAGICIAN_H, WHITE);
}

void C_Magician::Release_Image()
{
    SelectObject(memdc, oldBit);
    for (auto& image : MagicianBit)
        DeleteObject(image);
    DeleteDC(memdc);
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
