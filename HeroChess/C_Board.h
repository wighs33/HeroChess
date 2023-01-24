#pragma once
#include "C_Hero.h"
#include "C_Image.h"

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
	//무조건 인라인 함수여야 한다. (안 그러면 렌더링 안됨)
	C_Board()
		:grass_tile(RGB(0, 128, 0)),
		ground_tile(RGB(150, 75, 0)),
		river_tile(RGB(0, 10, 96))
	{
		Generate_Grid();

		p1_heroes[0] = make_shared<C_Magician>(Index_To_Pos(0), Index_To_Pos(1));
		p1_heroes[1] = make_shared<C_Reaper>(Index_To_Pos(1), Index_To_Pos(1));
		p1_heroes[2] = make_shared<C_Ninja>(Index_To_Pos(2), Index_To_Pos(1));
		p1_heroes[3] = make_shared<C_Ghost>(Index_To_Pos(3), Index_To_Pos(1));
		p1_heroes[4] = make_shared<C_Warrior>(Index_To_Pos(4), Index_To_Pos(1));
		p1_heroes[5] = make_shared<C_Defender>(Index_To_Pos(5), Index_To_Pos(1));
		p1_heroes[6] = make_shared<C_Knight>(Index_To_Pos(6), Index_To_Pos(1));

		p2_heroes[0] = make_shared<C_Magician>(Index_To_Pos(BOARD_W - 1), Index_To_Pos(BOARD_H - 2));
		p2_heroes[1] = make_shared<C_Reaper>(Index_To_Pos(BOARD_W - 2), Index_To_Pos(BOARD_H - 2));
		p2_heroes[2] = make_shared<C_Ninja>(Index_To_Pos(BOARD_W - 3), Index_To_Pos(BOARD_H - 2));
		p2_heroes[3] = make_shared<C_Ghost>(Index_To_Pos(BOARD_W - 4), Index_To_Pos(BOARD_H - 2));
		p2_heroes[4] = make_shared<C_Warrior>(Index_To_Pos(BOARD_W - 5), Index_To_Pos(BOARD_H - 2));
		p2_heroes[5] = make_shared<C_Defender>(Index_To_Pos(BOARD_W - 6), Index_To_Pos(BOARD_H - 2));
		p2_heroes[6] = make_shared<C_Knight>(Index_To_Pos(BOARD_W - 7), Index_To_Pos(BOARD_H - 2));
	}

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

	void Show_Color(HDC memdc, int rect_x, int rect_y, int color);

	void Check_Click(int x, int y);

	void Act_Hero();

	void Turn_Change();

	shared_ptr<C_Hero>& Find_Hero_By_Index(int who, int x, int y);

	pair<int, int> Lifes() const { return lifes; }

private:
	C_Grid grass_tile;
	C_Grid ground_tile;
	C_Grid river_tile;

	C_Grid* tiles_[BOARD_H][BOARD_W];

	shared_ptr<C_Hero> p1_heroes[BOARD_W];
	shared_ptr<C_Hero> p2_heroes[BOARD_W];

	// 0 : 빈 칸
	// 1 : Player1
	// 2 : Player2
	int heroes_pos[BOARD_H][BOARD_W];

	GamePlay gameplay;

	pair<int, int> click_index;

	int select_x, select_y;
	int selected_index = -1;

	//영웅 추가 시 생성자에서 할당
	static const int N_HEROES = 7;

	pair<int, int> lifes = { 2,2 };
};
