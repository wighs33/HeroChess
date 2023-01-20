#pragma once
#include "C_Hero.h"

class GamePlay
{
public:
	pair<int, int> turn_action = { 1, 0 };
	const enum ACTION
	{
		SELECT = 0,
		MOVE,
		SKILL
	};
};


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

	void Render(HDC memdc);
	void Render_Heroes(HDC memdc);

	int Pos_To_Index(int pos)
	{
		return pos / GRID_WH - 1;
	}

	int Index_To_Pos(int index)
	{
		return (index+1) * GRID_WH;
	}

	void Generate_Grid();

	//������ �ζ��� �Լ����� �Ѵ�. (�� �׷��� ������ �ȵ�)
	void Generate_Hero()
	{
		p1_heroes.resize(7);
		p1_heroes[0] = make_shared<C_Magician>(Index_To_Pos(0), Index_To_Pos(1));

		p2_heroes.resize(7);
		p2_heroes[0] = make_shared<C_Magician>(Index_To_Pos(BOARD_W - 1), Index_To_Pos(BOARD_H - 2));
	}

	void Show_Color(HDC memdc, int rect_x, int rect_y, int color);

	void Check_Click(int x, int y);

	void Act_Hero();

private:
	C_Grid grass_tile;
	C_Grid ground_tile;
	C_Grid river_tile;

	static const int BOARD_W = 7;
	static const int BOARD_H = 10;

	C_Grid* tiles_[BOARD_H][BOARD_W];

	vector<shared_ptr<C_Hero>> p1_heroes;
	vector<shared_ptr<C_Hero>> p2_heroes;

	GamePlay gameplay;

	pair<int, int> click_index;

	int Select_x, Select_y;
};
