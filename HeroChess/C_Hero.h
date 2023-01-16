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
    virtual void Load_Image() override;
    virtual void Render() override;
    virtual void Release_Image() override;
    virtual void Move_Per_Frame(int dest_x, int dest_y) override;

	virtual void Set_HDC(HDC& hdc) override { hdc_ = hdc; }
    virtual int get_x() const override { return x; }
    virtual int get_y() const override { return y; }

private:
    int x = 100;
    int y = 300;

    //���� ���۸�
    //�޽����� �߻��� ������ DC�� �ʱ�ȭ�ϹǷ�
    //��ü�ϴ� ����� �ƴ� �׸��� DC�� ȭ�鿡 ǥ���ϴ� DC�� ���� ���صδ� �� ����.
    HDC hdc_;
    HDC memdc;
    HBITMAP MagicianBit[17], oldBit;

    int count = 0;
    const int MAGICIAN_W = 48;
    const int MAGICIAN_H = 48;
};