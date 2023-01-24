#pragma once
#include "resource.h"

class C_Image
{
public:
	C_Image(int image, int w, int h) : image_(image), image_w(w), image_h(h)
	{
		BackgroundBit = LoadBitmap(hInst, MAKEINTRESOURCE(image_));
	}

	~C_Image()
	{
		DeleteObject(BackgroundBit);
	}

	void Render(HDC memdc, int sx, int sy, int ex, int ey);

private:
	int image_;
	int image_w;
	int image_h;

	HBITMAP BackgroundBit;
};


