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
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % N_IMAGES;
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

void C_Magician::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //�ִϸ��̼� ���
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //������ �ɷ� : ��ġ ü����
    int tmp_x = x_;
    int tmp_y = y_;

    x_ = hero->get_x();
    y_ = hero->get_y();

    hero->set_x(tmp_x);
    hero->set_y(tmp_y);
}

void C_Reaper::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, ReaperBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, REAPER_W, REAPER_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}

void C_Reaper::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //�ִϸ��̼� ���
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //��� �ɷ� : ���� ���� �Ѹ� ����
    hero.reset();
    hero = make_shared<C_None>(0, 0);
}

void C_Ninja::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, NinjaBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, NINJA_W, NINJA_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}

void C_Ninja::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //�ִϸ��̼� ���
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
}

void C_Ghost::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, GhostBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, GHOST_W, GHOST_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}

void C_Ghost::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //�ִϸ��̼� ���
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //��Ʈ �ɷ� : �� ���� ���� ���� �� �� �� �����ϰ� �ʱ���ġ�� ��ȯ
    hero.reset();
    hero = make_shared<C_None>(0, 0);
}

void C_Warrior::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, WarriorBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, WARRIOR_W, WARRIOR_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}

void C_Warrior::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //���� Ŭ�������� �ذ�
}

void C_Defender::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, DefenderBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 0, 0, DEFENDER_W, DEFENDER_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}

void C_Defender::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //Move_Per_Frame �޼���� �ذ�
}

void C_Knight::Render(HDC memdc)
{
    //�ִϸ��̼� ī��Ʈ ������Ʈ �ϱ�
    if (move_ == 1)
        count = ++count % N_IMAGES;
    else
        count = 0;

    //�̹���DC ����
    HDC image_dc = CreateCompatibleDC(memdc);
    //�ش� �ִϸ��̼� ������ �̹���DC�� �ֱ�
    HBITMAP oldBit = (HBITMAP)SelectObject(image_dc, KnightBit[count]);

    //�̹���DC�� ������ �ӽ�DC�� ������ ���� �ֱ�
    //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
    TransparentBlt(memdc, x_, y_, GRID_WH, GRID_WH, image_dc, 10, 10, KNIGHT_W, KNIGHT_H, WHITE);

    //�̹��� �����
    SelectObject(image_dc, oldBit);
    //�̹���DC ����
    DeleteDC(image_dc);
}

void C_Knight::Use_Skill(shared_ptr<C_Hero>& hero)
{
    //�ִϸ��̼� ���
    move_ = 1;
    if (count == N_IMAGES - 1)
    {
        move_ = 0;
        count = 0;
    }
    if (move_ == 1) return;

    //������ �ɷ� : ��ġ ü����
}