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
	//������ �ζ��� �Լ����� �Ѵ�. (�� �׷��� ������ �ȵ�)
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

	C_Board& operator=(const C_Board& newboard)
	{
		grass_tile = newboard.grass_tile;
		ground_tile = newboard.ground_tile;
		river_tile = newboard.river_tile;

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

		gameplay.turn_action.second = 0;
		click_index = { -1, -1 };
		select_x = 0;
		select_y = 0;
		selected_index = -1;
		lifes = { 2,2 };
		skill_copy_index = -1;
		is_select_copy = false;

		return *this;
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

	void Magician_Skill_Range(HDC memdc, int except);
	void Reaper_And_Ninja_Skill_Range(HDC memdc);
	void Ghost_Skill_Range(HDC memdc);
	void Defender_Skill_Range(HDC memdc);
	void Knight_Skill_Range(HDC memdc);

	void Magician_Skill();
	void Reaper_Skill();
	void Ninja_Skill();
	void Ghost_Skill();
	void Warrior_Skill();
	void Defender_Skill();
	void Knight_Skill();

	void Text_For_Debug(HDC memdc);
	void Skill_Desc(HDC memdc);

private:
	C_Grid grass_tile;
	C_Grid ground_tile;
	C_Grid river_tile;

	C_Grid* tiles_[BOARD_H][BOARD_W];

	shared_ptr<C_Hero> p1_heroes[BOARD_W];
	shared_ptr<C_Hero> p2_heroes[BOARD_W];

	// 0 : �� ĭ
	// 1 : Player1
	// 2 : Player2
	int heroes_pos[BOARD_H][BOARD_W];

	GamePlay gameplay;

	pair<int, int> click_index;

	int select_x, select_y;
	int selected_index = -1;

	//���� �߰� �� �����ڿ��� �Ҵ�
	static const int N_HEROES = 7;

	pair<int, int> lifes = { 2,2 };

	shared_ptr<C_Hero> null_hero = nullptr;
	int skill_copy_index = -1;
	bool is_select_copy = false;

	shared_ptr<C_Hero>* heroes;
	shared_ptr<C_Hero>* opposing_heroes;
};
