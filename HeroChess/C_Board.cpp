#include "stdafx.h"
#include "C_Board.h"

void C_Board::Render(HDC memdc)
{
	//타일 정보에 따라 색칠 하기
	for (size_t h = 0; h < BOARD_H; h++)
		for (size_t w = 0; w < BOARD_W; w++)
		{
			//타일의 색정보를 넘겨준다.
			HBRUSH hbrush = CreateSolidBrush(tiles_[h][w]->Get_Color());
			HBRUSH oldbrush = (HBRUSH)SelectObject(memdc, hbrush);
			Rectangle(memdc, Index_To_Pos(w), Index_To_Pos(h), Index_To_Pos(w + 1), Index_To_Pos(h + 1));
			SelectObject(memdc, oldbrush);
			DeleteObject(hbrush);
		}

	//격자 그리기
	HPEN hpen = CreatePen(PS_SOLID, 5, BLACK_BRUSH);
	HPEN oldbrush = (HPEN)SelectObject(memdc, hpen);
	for (int i = GRID_WH; i <= GRID_WH * 8; i += GRID_WH)
	{
		//가로
		//시작점
		MoveToEx(memdc, i, GRID_WH, NULL);
		//끝점
		LineTo(memdc, i, GRID_WH * 11);
	}

	for (int i = GRID_WH; i <= GRID_WH * 11; i += GRID_WH)
	{
		//세로
		//시작점
		MoveToEx(memdc, GRID_WH, i, NULL);
		//끝점
		LineTo(memdc, GRID_WH * 8, i);
	}
	SelectObject(memdc, oldbrush);
	DeleteObject(hpen);
}

void C_Board::Render_Heroes(HDC memdc)
{
	//영웅 들 그리기
	p1_heroes[0]->Render(memdc);
	p2_heroes[0]->Render(memdc);

	int hero_x = p1_heroes[0]->get_x();
	int hero_y = p1_heroes[0]->get_y();

	if (p1_heroes[0]->Get_Move() == 1) return;

	//색 표시
	if (gameplay.turn_action == make_pair<int, int>(1, gameplay.SELECT))
	{
		Show_Color(memdc, hero_x, hero_y, BLUE);
	}
	else if (gameplay.turn_action == make_pair<int, int>(1, gameplay.MOVE))
	{
		Show_Color(memdc, hero_x, hero_y - GRID_WH, GREEN);
		Show_Color(memdc, hero_x, hero_y + GRID_WH, GREEN);
		Show_Color(memdc, hero_x - GRID_WH, hero_y, GREEN);
		Show_Color(memdc, hero_x + GRID_WH, hero_y, GREEN);
	}
	else 	if (gameplay.turn_action == make_pair<int, int>(1, gameplay.SKILL))
	{
		//바꿔야됨
		Show_Color(memdc, hero_x, hero_y, YELLOW);
	}
}

void C_Board::Generate_Grid()
{
	for (size_t h = 0; h < BOARD_H; h++)
		for (size_t w = 0; w < BOARD_W; w++)
		{
			//랜덤 1/5확률
			if (uid(dre) == 0)
			{
				tiles_[h][w] = &ground_tile;
			}
			else
			{
				tiles_[h][w] = &grass_tile;
			}
		}

	int h = uid(dre);
	for (size_t w = 0; w < BOARD_W; w++)
	{
		tiles_[h][w] = &river_tile;
	}
}

void C_Board::Show_Color(HDC memdc, int rect_x, int rect_y, int color)
{
	if (Pos_To_Index(rect_x) < 0 || Pos_To_Index(rect_x) >= BOARD_W
		|| Pos_To_Index(rect_y) < 0 || Pos_To_Index(rect_y) >= BOARD_H)
		return;
	//사각형그릴때  투명으로
	SetROP2(memdc, R2_XORPEN);
	SelectObject(memdc, (HBRUSH)GetStockObject(BLACK_BRUSH));

	HPEN hpen = CreatePen(PS_SOLID, 5, color);
	HPEN oldbrush = (HPEN)SelectObject(memdc, hpen);

	Rectangle(memdc, rect_x, rect_y, rect_x + GRID_WH, rect_y + GRID_WH);

	SelectObject(memdc, oldbrush);
	DeleteObject(hpen);
}

void C_Board::Check_Click(int x, int y)
{
	cout << x << ' ' << y << endl;

	click_index = { -1, -1 };

	//click_index에 클릭한 칸의 인덱스 넣기
	for (size_t i = 0; i < BOARD_W; i++)
	{
		if (Index_To_Pos(i) <= x and x <= Index_To_Pos(i+1))
		{
			click_index.first = i;
		}
	}

	for (size_t i = 0; i < BOARD_H; i++)
	{
		if (Index_To_Pos(i) <= y and y <= Index_To_Pos(i + 1))
		{
			click_index.second = i;
		}
	}
}

void C_Board::Act_Hero()
{
	//보드 밖 클릭하면 리턴
	if (click_index.first == -1 || click_index.second == -1) return;

	//선택 페이즈
	if (gameplay.turn_action == make_pair<int, int>(1, gameplay.SELECT))
	{
		if (click_index == make_pair<int, int>(Pos_To_Index(p1_heroes[0]->get_x()), Pos_To_Index(p1_heroes[0]->get_y())))
		{
			//클릭 위치 저장
			Select_x = p1_heroes[0]->get_x();
			Select_y = p1_heroes[0]->get_y();
			//이동 페이즈로 전환
			++gameplay.turn_action.second;
		}
	}
	//이동 페이즈
	else if (gameplay.turn_action == make_pair<int, int>(1, gameplay.MOVE))
	{
		//상
		if (click_index == make_pair<int, int>(Pos_To_Index(Select_x), Pos_To_Index(Select_y) - 1))
		{
			//이동할 때까지 기다리고 이동 다하면 다음 페이즈로
			p1_heroes[0]->Set_Move(1);
			p1_heroes[0]->Move_Per_Frame(Select_x, Select_y - GRID_WH);
			if (p1_heroes[0]->Get_Move() == 2)
			{
				++gameplay.turn_action.second;
				p1_heroes[0]->Set_Move(0);
			}
		}
		//하
		else if (click_index == make_pair<int, int>(Pos_To_Index(Select_x), Pos_To_Index(Select_y) + 1))
		{
			//이동할 때까지 기다리고 이동 다하면 다음 페이즈로
			p1_heroes[0]->Set_Move(1);
			p1_heroes[0]->Move_Per_Frame(Select_x, Select_y + GRID_WH);
			if (p1_heroes[0]->Get_Move() == 2)
			{
				++gameplay.turn_action.second;
				p1_heroes[0]->Set_Move(0);
			}
		}
		//좌
		else if (click_index == make_pair<int, int>(Pos_To_Index(Select_x) - 1, Pos_To_Index(Select_y)))
		{
			//이동할 때까지 기다리고 이동 다하면 다음 페이즈로
			p1_heroes[0]->Set_Move(1);
			p1_heroes[0]->Move_Per_Frame(Select_x - GRID_WH, Select_y);
			if (p1_heroes[0]->Get_Move() == 2)
			{
				++gameplay.turn_action.second;
				p1_heroes[0]->Set_Move(0);
			}
		}
		//우
		else if (click_index == make_pair<int, int>(Pos_To_Index(Select_x) + 1, Pos_To_Index(Select_y)))
		{
			//이동할 때까지 기다리고 이동 다하면 다음 페이즈로
			p1_heroes[0]->Set_Move(1);
			p1_heroes[0]->Move_Per_Frame(Select_x + GRID_WH, Select_y);
			if (p1_heroes[0]->Get_Move() == 2)
			{
				++gameplay.turn_action.second;
				p1_heroes[0]->Set_Move(0);
			}
		}
	}
	//능력 사용 페이즈
	else 	if (gameplay.turn_action == make_pair<int, int>(1, gameplay.SKILL))
	{
		//if (hero_x <= x and x <= hero_x + GRID_WH and hero_y <= y and y <= hero_y + GRID_WH)
		//{
		//	++gameplay.turn_action.first;
		//}
	}

	//click_index = { -1, -1 };
}