#pragma once

class C_Grid
{
public:
	C_Grid(int color)
		:color_(color)
	{}

	int Get_Color() const { return color_; }

private:
	int color_;
};

class C_Board
{
public:
	C_Board()
		:grass_tile(RGB(0, 128, 0)),
		ground_tile(RGB(150, 75, 0)),
		river_tile(RGB(0, 10, 96))
	{}

	void Render(HDC hdc);

	int Pos_To_Index(int pos)
	{
		return pos / 60 - 1;
	}

	int Index_To_Pos(int index)
	{
		return (index+1) * 60;
	}

	void Generate_Grid();
private:
	C_Grid grass_tile;
	C_Grid ground_tile;
	C_Grid river_tile;

	static const int BOARD_W = 7;
	static const int BOARD_H = 10;

	C_Grid* tiles_[BOARD_H][BOARD_W];
};
