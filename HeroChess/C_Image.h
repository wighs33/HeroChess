#pragma once
#include "resource.h"

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

    void Render(HDC memdc);

private:
    int image_;
    const int IMAGE_W = 320;
    const int IMAGE_H = 320;

    HBITMAP BackgroundBit;
};


