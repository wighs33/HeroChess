#pragma once
#include "Resource.h"

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

    const int MAGICIAN_W = 43;
    const int MAGICIAN_H = 43;

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

    inline static const int start = IDB_BITMAP23;
};

class C_Ghost : public C_Hero
{
public:
    C_Ghost(int x, int y)
        : C_Hero(x, y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            GhostBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(start + i));
    }
    virtual ~C_Ghost() override
    {
        for (auto& image : GhostBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override
    {
        C_Hero::Move_Per_Frame(dest_x, dest_y);
    }
private:
    static const int N_IMAGES = 6;

    HBITMAP GhostBit[N_IMAGES];

    const int GHOST_W = 48;
    const int GHOST_H = 48;

    inline static const int start = IDB_BITMAP32;
};

class C_Warrior : public C_Hero
{
public:
    C_Warrior(int x, int y)
        : C_Hero(x, y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            WarriorBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(start + i));
    }
    virtual ~C_Warrior() override
    {
        for (auto& image : WarriorBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override
    {
        C_Hero::Move_Per_Frame(dest_x, dest_y);
    }
private:
    static const int N_IMAGES = 5;

    HBITMAP WarriorBit[N_IMAGES];

    const int WARRIOR_W = 100;
    const int WARRIOR_H = 100;

    inline static const int start = IDB_BITMAP38;
};

class C_Defender : public C_Hero
{
public:
    C_Defender(int x, int y)
        : C_Hero(x, y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            DefenderBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(start + i));
    }
    virtual ~C_Defender() override
    {
        for (auto& image : DefenderBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override
    {
        C_Hero::Move_Per_Frame(dest_x, dest_y);
    }
private:
    static const int N_IMAGES = 3;

    HBITMAP DefenderBit[N_IMAGES];

    const int DEFENDER_W = 140;
    const int DEFENDER_H = 140;

    inline static const int start = IDB_BITMAP43;
};

class C_Knight : public C_Hero
{
public:
    C_Knight(int x, int y)
        : C_Hero(x, y)
    {
        for (int i = 0; i < N_IMAGES; i++)
            KnightBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(start + i));
    }
    virtual ~C_Knight() override
    {
        for (auto& image : KnightBit)
            DeleteObject(image);
    }
    virtual void Render(HDC memdc) override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override
    {
        C_Hero::Move_Per_Frame(dest_x, dest_y);
    }
private:
    static const int N_IMAGES = 4;

    HBITMAP KnightBit[N_IMAGES];

    const int KNIGHT_W = 80;
    const int KNIGHT_H = 80;

    inline static const int start = IDB_BITMAP46;
};