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

    HBITMAP MagicianBit[17];

    int count = 0;
    const int MAGICIAN_W = 48;
    const int MAGICIAN_H = 48;
};