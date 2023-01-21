#pragma once
#include "Resource.h"
#include <cmath>

class C_Hero
{
public:
    C_Hero(int x, int y) : x_(x), y_(y) {};
    virtual ~C_Hero() {}
    virtual void Render(HDC memdc) = 0;
    virtual void Move_Per_Frame(int destX, int destY);
    virtual int get_x() const { return x_; }
    virtual int get_y() const { return y_; }
    virtual void Set_Move(int move) { move_ = move; }
    virtual int Get_Move() const { return move_; }

protected:
    int x_;
    int y_;
    int move_ = 0;
    int count = 0;
};

class C_Magician : public C_Hero
{
public:
    C_Magician(int x, int y) 
		: C_Hero(x, y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            MagicianBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(start + i));
    }
    virtual ~C_Magician() override
    {
        for (auto& image : MagicianBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override
    {
        C_Hero::Move_Per_Frame(dest_x, dest_y);
    }
private:
    static const int N_IMAGES = 16;

    HBITMAP MagicianBit[N_IMAGES];

    const int MAGICIAN_W = 48;
    const int MAGICIAN_H = 48;

    inline static const int start = IDB_BITMAP2;
};

class C_Reaper : public C_Hero
{
public:
    C_Reaper(int x, int y)
        : C_Hero(x, y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            ReaperBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(start + i));
    }
    virtual ~C_Reaper() override
    {
        for (auto& image : ReaperBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override
    {
        C_Hero::Move_Per_Frame(dest_x, dest_y);
    }
private:
    static const int N_IMAGES = 5;

    HBITMAP ReaperBit[N_IMAGES];

    const int REAPER_W = 70;
    const int REAPER_H = 75;

    inline static const int start = IDB_BITMAP18;
};

class C_Ninja : public C_Hero
{
public:
    C_Ninja(int x, int y)
        : C_Hero(x, y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            NinjaBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(start + i));
    }
    virtual ~C_Ninja() override
    {
        for (auto& image : NinjaBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override
    {
        C_Hero::Move_Per_Frame(dest_x, dest_y);
    }
private:
    static const int N_IMAGES = 9;

    HBITMAP NinjaBit[N_IMAGES];

    const int NINJA_W = 160;
    const int NINJA_H = 160;

    //юс╫ц
    inline static const int start = IDB_BITMAP18;
};