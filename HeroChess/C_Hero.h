#pragma once
#include "Resource.h"

class C_Hero
{
public:
    virtual ~C_Hero() {}
    virtual void Load_Image() = 0;
    virtual void Render() = 0;
    virtual void Release_Image() = 0;
    virtual void Move_Per_Frame() = 0;
    virtual void Set_HDC(HDC& hdc) = 0;
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

    virtual void Move_Per_Frame() override
    {
        x += 10;
        if (x > 819) x = -100;
    }

    virtual void Set_HDC(HDC& hdc) override
    {
        hdc_ = hdc;
    }

private:
    int x = -100;
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