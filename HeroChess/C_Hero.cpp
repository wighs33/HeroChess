#include "stdafx.h"
#include "C_Hero.h"
#include <cmath>

void C_Hero::Move_Per_Frame(int dest_x, int dest_y)
{
    move_ = 1;
    if (x_ == dest_x and y_ == dest_y) move_ = 2;
    if (move_ == 2) return;

    int dist_x = dest_x - x_;
    int dist_y = dest_y - y_;

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
    //애니메이션 카운트 업데이트 하기
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, MagicianBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, MAGICIAN_W, MAGICIAN_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
    DeleteDC(image_dc);
}

void C_Magician::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //애니메이션 재생
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //마법사 능력 : 위치 체인지
    int tmp_x = x_;
    int tmp_y = y_;

    x_ = hero->get_x();
    y_ = hero->get_y();

    hero->set_x(tmp_x);
    hero->set_y(tmp_y);
}

void C_Reaper::Render(HDC memdc)
{
    //애니메이션 카운트 업데이트 하기
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, ReaperBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, REAPER_W, REAPER_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
    DeleteDC(image_dc);
}

void C_Reaper::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //애니메이션 재생
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //사신 능력 : 주위 영웅 한명 제거
    hero.reset();
    hero = make_shared<C_None>(0, 0);
}

void C_Ninja::Render(HDC memdc)
{
    //애니메이션 카운트 업데이트 하기
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, NinjaBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, NINJA_W, NINJA_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
    DeleteDC(image_dc);
}

void C_Ninja::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //애니메이션 재생
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
}

void C_Ghost::Render(HDC memdc)
{
    //애니메이션 카운트 업데이트 하기
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, GhostBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, GHOST_W, GHOST_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
    DeleteDC(image_dc);
}

void C_Ghost::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //애니메이션 재생
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //고스트 능력 : 적 진영 끝에 도달 시 한 명 제거하고 초기위치로 귀환
    hero.reset();
    hero = make_shared<C_None>(0, 0);
}

void C_Warrior::Render(HDC memdc)
{
    //애니메이션 카운트 업데이트 하기
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, WarriorBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, WARRIOR_W, WARRIOR_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
    DeleteDC(image_dc);
}

void C_Warrior::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //보드 클래스에서 해결
}

void C_Defender::Render(HDC memdc)
{
    //애니메이션 카운트 업데이트 하기
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, DefenderBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, DEFENDER_W, DEFENDER_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
    DeleteDC(image_dc);
}

void C_Defender::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //Move_Per_Frame 메서드로 해결
}

void C_Knight::Render(HDC memdc)
{
    //애니메이션 카운트 업데이트 하기
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //이미지DC 생성
    HDC image_dc = CreateCompatibleDC(memdc);
    //해당 애니메이션 프레임 이미지DC에 넣기
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, KnightBit[count]);

    //이미지DC의 사진을 임시DC의 적절한 곳에 넣기
    //정확한 이미지 크기가 들어가야 제대로 나온다.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 10, 10, KNIGHT_W, KNIGHT_H, WHITE);

    //이미지 지우기
    SelectObject(image_dc, oldBit);
    //이미지DC 삭제
    DeleteDC(image_dc);
}

void C_Knight::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //애니메이션 재생
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //마법사 능력 : 위치 체인지
}