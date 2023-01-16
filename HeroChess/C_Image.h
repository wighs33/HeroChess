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
        //정확한 이미지 크기가 들어가야 제대로 나온다.
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

    //더블 버퍼링
    //메시지가 발생할 때마다 DC가 초기화하므로
    //교체하는 방식이 아닌 그리는 DC와 화면에 표시하는 DC를 따로 정해두는 게 좋다.
    HDC hdc_;
    HDC memdc;
    HBITMAP BackgroundBit, oldBit;
};


