#pragma once
#include "Resource.h"
#include <cmath>

class C_Hero
{
public:
    virtual ~C_Hero() {}
    virtual void Render(HDC hdc) = 0;
    virtual void Move_Per_Frame(int destX, int destY) = 0;
    virtual int get_x() const = 0;
    virtual int get_y() const = 0;
};

class C_Magician : public C_Hero
{
public:
    C_Magician() 
    {
        for (int i = 0; i < 17; i++)
            MagicianBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2 + i));
    }
    virtual ~C_Magician() override
    {
        for (auto& image : MagicianBit)
            DeleteObject(image);
    }
    virtual void Render(HDC hdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override;

    virtual int get_x() const override { return x; }
    virtual int get_y() const override { return y; }

private:
    int x = 100;
    int y = 300;

    //더블 버퍼링
    //메시지가 발생할 때마다 DC가 초기화하므로
    //교체하는 방식이 아닌 그리는 DC와 화면에 표시하는 DC를 따로 정해두는 게 좋다.
    HDC memdc;
    HBITMAP MagicianBit[17], oldBit;

    int count = 0;
    const int MAGICIAN_W = 48;
    const int MAGICIAN_H = 48;
};