#pragma once
#include "Resource.h"
#include <cmath>

class C_Hero
{
public:
    virtual ~C_Hero() {}
    virtual void Load_Image() = 0;
    virtual void Render() = 0;
    virtual void Release_Image() = 0;
    virtual void Move_Per_Frame(int destX, int destY) = 0;
    virtual void Set_HDC(HDC& hdc) = 0;
    virtual int get_x() const = 0;
    virtual int get_y() const = 0;
};

class C_Magician : public C_Hero
{
public:
    virtual void Load_Image() override
    {
        count = ++count % 16;
        for (int i = 0; i < 17; i++)
            MagicianBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2 + i));
    }

    virtual void Render() override
    {
        memdc = CreateCompatibleDC(hdc_);
        oldBit = (HBITMAP)SelectObject(memdc, MagicianBit[count]);
        //정확한 이미지 크기가 들어가야 제대로 나온다.
        TransparentBlt(hdc_, x, y, MAGICIAN_W, MAGICIAN_H, memdc, 0, 0, MAGICIAN_W, MAGICIAN_H, WHITE);
    }

    virtual void Release_Image() override
    {
        SelectObject(memdc, oldBit);
        for (auto& image : MagicianBit)
            DeleteObject(image);
        DeleteDC(memdc);
    }

    virtual void Move_Per_Frame(int dest_x, int dest_y) override
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

    virtual void Set_HDC(HDC& hdc) override
    {
        hdc_ = hdc;
    }

    virtual int get_x() const override { return x; }
    virtual int get_y() const override { return y; }

private:
    int x = 100;
    int y = 300;

    //더블 버퍼링
    //메시지가 발생할 때마다 DC가 초기화하므로
    //교체하는 방식이 아닌 그리는 DC와 화면에 표시하는 DC를 따로 정해두는 게 좋다.
    HDC hdc_;
    HDC memdc;
    HBITMAP MagicianBit[17], oldBit;

    int count = 0;
    const int MAGICIAN_W = 48;
    const int MAGICIAN_H = 48;
};