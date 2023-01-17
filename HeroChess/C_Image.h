#pragma once

class C_Image
{
public:
    C_Image(int image) : image_(image) 
    {
        BackgroundBit = LoadBitmap(hInst, MAKEINTRESOURCE(image_));
    }

    ~C_Image() 
    {
        DeleteObject(BackgroundBit); 
    }

    void Render(HDC hdc);

private:
    int image_ = 0;
    const int IMAGE_W = 320;
    const int IMAGE_H = 320;

    //더블 버퍼링
    //메시지가 발생할 때마다 DC가 초기화하므로
    //교체하는 방식이 아닌 그리는 DC와 화면에 표시하는 DC를 따로 정해두는 게 좋다.
    HDC memdc;
    HBITMAP BackgroundBit, oldBit;
};


