#include "stdafx.h"
#include "C_Board.h"

void C_Board::Generate_Grid()
{
	for (size_t h = 0; h < BOARD_H; h++)
	{
		for (size_t w = 0; w < BOARD_W; w++)
		{
			if (uid(dre) == 0)
			{
				tiles_[h][w] = &hill_tile;
			}
			else
			{
				tiles_[h][w] = &grass_tile;
			}
		}
	}

	int h = uid(dre);
	for (size_t w = 0; w < BOARD_W; w++)
	{
		tiles_[h][w] = &river_tile;
	}
}
