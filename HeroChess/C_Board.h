#pragma once

class C_Grid
{
public:
	C_Grid(int move_cost, bool is_water)
		:move_cost_(move_cost), is_water_(is_water)
	{}

	int Get_Move_Cost() const { return move_cost_; }
	bool Is_Water() const { return is_water_; }

private:
	int move_cost_;
	bool is_water_;
	//Texture texture_;
};

class C_Board
{
public:
	C_Board()
		:grass_tile(1, false),
		hill_tile(3, false),
		river_tile(2, true)
	{}

	void Generate_Grid();
private:
	C_Grid grass_tile;
	C_Grid hill_tile;
	C_Grid river_tile;

	static const int BOARD_W = 7;
	static const int BOARD_H = 10;

	C_Grid* tiles_[BOARD_H][BOARD_W];
};

