#pragma once
#include "Resource.h"
#include <cmath>

class C_Hero
{
public:
    virtual ~C_Hero() {}
    virtual void Render(HDC memdc) = 0;
    virtual void Move_Per_Frame(int destX, int destY) = 0;
    virtual int get_x() const = 0;
    virtual int get_y() const = 0;
    virtual void Set_Move(int move) = 0;
    virtual int Get_Move() const = 0;
};

class C_Magician : public C_Hero
{
public:
    C_Magician(int x, int y) 
        : x_(x), y_(y)
    {
        for (int i = 0; i < 17; i++)
            MagicianBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2 + i));
    }
    virtual ~C_Magician() override
    {
        for (auto& image : MagicianBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override;

    virtual int get_x() const override { return x_; }
    virtual int get_y() const override { return y_; }

    virtual void Set_Move(int move) override { move_ = move; }
    virtual int Get_Move() const override { return move_; }
private:
    int x_;
    int y_;

    static const int N_IMAGES = 17;

    HBITMAP MagicianBit[N_IMAGES];

    int count = 0;
    const int MAGICIAN_W = 48;
    const int MAGICIAN_H = 48;
    
    int move_ = 0;
};

class C_Reaper : public C_Hero
{
public:
    C_Reaper(int x, int y)
        : x_(x), y_(y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            ReaperBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP19 + i));
    }
    virtual ~C_Reaper() override
    {
        for (auto& image : ReaperBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override;

    virtual int get_x() const override { return x_; }
    virtual int get_y() const override { return y_; }

    virtual void Set_Move(int move) override { move_ = move; }
    virtual int Get_Move() const override { return move_; }
private:
    int x_;
    int y_;

    static const int N_IMAGES = 5;

    HBITMAP ReaperBit[N_IMAGES];

    int count = 0;
    const int REAPER_W = 70;
    const int REAPER_H = 75;

    int move_ = 0;
};