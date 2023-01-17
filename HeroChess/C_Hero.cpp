#include "stdafx.h"
#include "C_Hero.h"

void C_Magician::Render(HDC memdc)
{
    //애니메이션 카운트 업데이트 하기
    count = ++count % 16;
    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, MagicianBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x, y, MAGICIAN_W, MAGICIAN_H, image_dc, 0, 0, MAGICIAN_W, MAGICIAN_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
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
