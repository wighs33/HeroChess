#include "stdafx.h"
#include "C_Board.h"

void C_Board::Render(HDC memdc)
{
	for (size_t h = 0; h < BOARD_H; h++)
	{
		for (size_t w = 0; w < BOARD_W; w++)
		{
			HBRUSH hbrush = CreateSolidBrush(tiles_[h][w]->Get_Color());
			HBRUSH oldbrush = (HBRUSH)SelectObject(memdc, hbrush);
			Rectangle(memdc, Index_To_Pos(w), Index_To_Pos(h), Index_To_Pos(w + 1), Index_To_Pos(h + 1));
			SelectObject(memdc, oldbrush);
			DeleteObject(hbrush);
		}
	}

	HPEN hpen = CreatePen(PS_SOLID, 5, RGB(200, 200, 200));
	HPEN oldbrush = (HPEN)SelectObject(memdc, hpen);
	for (int i = 60; i <= 60 * 8; i += 60)
	{
		//가로
		MoveToEx(memdc, i, 60, NULL);
		LineTo(memdc, i, 60 * 11);
	}

	for (int i = 60; i <= 60 * 11; i += 60)
	{
		//세로
		MoveToEx(memdc, 60, i, NULL);
		LineTo(memdc, 60 * 8, i);
	}
	SelectObject(memdc, oldbrush);
	DeleteObject(hpen);
}

void C_Board::Generate_Grid()
{
	for (size_t h = 0; h < BOARD_H; h++)
	{
		for (size_t w = 0; w < BOARD_W; w++)
		{
			if (uid(dre) == 0)
			{
				tiles_[h][w] = &ground_tile;
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
