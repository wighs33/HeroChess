#pragma once

class C_Image
{
public:
    C_Image(int image) : image_(image) {}

    void Load_Image()
    {
        BackgroundBit = LoadBitmap(hInst, MAKEINTRESOURCE(image_));
    }

    void Render()
    {
        memdc = CreateCompatibleDC(hdc_);
        oldBit = (HBITMAP)SelectObject(memdc, BackgroundBit);
        //��Ȯ�� �̹��� ũ�Ⱑ ���� ����� ���´�.
        StretchBlt(hdc_, 0, 0, WIN_W - 10, WIN_H - 50, memdc, 0, 0, IMAGE_W, IMAGE_H, SRCCOPY);
    }

    void Release_Image()
    {
        SelectObject(memdc, oldBit);
        DeleteObject(BackgroundBit);
        DeleteDC(memdc);
    }

    void Set_HDC(HDC& hdc)
    {
        hdc_ = hdc;
    }

private:
    int image_ = 0;
    const int IMAGE_W = 320;
    const int IMAGE_H = 320;

    //���� ���۸�
    //�޽����� �߻��� ������ DC�� �ʱ�ȭ�ϹǷ�
    //��ü�ϴ� ����� �ƴ� �׸��� DC�� ȭ�鿡 ǥ���ϴ� DC�� ���� ���صδ� �� ����.
    HDC hdc_;
    HDC memdc;
    HBITMAP BackgroundBit, oldBit;
};


