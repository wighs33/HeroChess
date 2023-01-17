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

    //���� ���۸�
    //�޽����� �߻��� ������ DC�� �ʱ�ȭ�ϹǷ�
    //��ü�ϴ� ����� �ƴ� �׸��� DC�� ȭ�鿡 ǥ���ϴ� DC�� ���� ���صδ� �� ����.
    HDC memdc;
    HBITMAP BackgroundBit, oldBit;
};


