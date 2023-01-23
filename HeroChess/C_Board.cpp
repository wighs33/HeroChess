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

	for (int iy = 0; iy < 10; ++iy)
		for (int ix = 0; ix < 7; ++ix)
		{
			static TCHAR lpOut[100];
			wsprintf(lpOut, _T("%d "), heroes_pos[iy][ix]);
			TextOut(memdc, Index_To_Pos(ix), Index_To_Pos(iy), lpOut, lstrlen(lpOut));
		}

	static TCHAR lpOut2[100];
	wsprintf(lpOut2, _T("%d %d"), gameplay.turn_action.first, gameplay.turn_action.second);
	TextOut(memdc, 20, 20, lpOut2, lstrlen(lpOut2));

	static TCHAR lpOut3[100];
	wsprintf(lpOut3, _T("%d %d"), click_index.first, click_index.second);
	TextOut(memdc, 20, 50, lpOut3, lstrlen(lpOut3));

	SelectObject(memdc, oldbrush);
	DeleteObject(hpen);
}

void C_Board::Render_Heroes(HDC memdc)
{
	auto heroes = p1_heroes;
	if(gameplay.turn_action.first == 2)
		heroes = p2_heroes;

	bool is_moving = false;
	for (size_t i = 0; i < N_HEROES; i++)
	{

		//영웅 들 그리기
		p1_heroes[i]->Render(memdc);
		p2_heroes[i]->Render(memdc);

		if (heroes[i]->Get_Move() == 1) is_moving = true;
	}

	//한 명이라도 이동 중일 때 색 표시 건너뛰기
	if (is_moving)
	{
		is_moving = false;
		return;
	}

	//색 표시
	if (gameplay.turn_action.second == gameplay.SELECT)
	{
		for (size_t i = 0; i < N_HEROES; i++)
			Show_Color(memdc, heroes[i]->get_x(), heroes[i]->get_y(), BLUE);
	}
	else if (gameplay.turn_action.second == gameplay.MOVE)
	{
		pair<int, int> four_dir[4];
		four_dir[0] = { select_x, select_y - GRID_WH };
		four_dir[1] = { select_x, select_y + GRID_WH };
		four_dir[2] = { select_x - GRID_WH, select_y };
		four_dir[3] = { select_x + GRID_WH, select_y };

		for (size_t dir_index = 0; dir_index < 4; dir_index++)
		{
			int tmp_x = Pos_To_Index(four_dir[dir_index].first);
			int tmp_y = Pos_To_Index(four_dir[dir_index].second);

			//위치를 간략화한 배열로 시간복잡도 줄이기
			if (heroes_pos[tmp_y][tmp_x] == 1) continue;

			Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, GREEN);
		}
	}
	else if (gameplay.turn_action.second == gameplay.SKILL)
	{
		if (selected_index == 0)
		{
			for (size_t i = 1; i < N_HEROES; i++)
			{
				Show_Color(memdc, heroes[i]->get_x(), heroes[i]->get_y(), YELLOW);
			}
		}
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

	//영웅 위치값 저장
	for (size_t i = 0; i < N_HEROES; i++)
	{
		heroes_pos[1][i] = 1;
		heroes_pos[BOARD_H - 2][i] = 2;
	}
}

void C_Board::Show_Color(HDC memdc, int rect_x, int rect_y, int color)
{
	if (rect_x == 0 and rect_y == 0) return;

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

	auto heroes = p1_heroes;
	if (gameplay.turn_action.first == 2)
		heroes = p2_heroes;


	//선택 페이즈
	if (gameplay.turn_action.second == gameplay.SELECT)
	{
		if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first)
		{
			//클릭 위치 저장
			select_x = Index_To_Pos(click_index.first);
			select_y = Index_To_Pos(click_index.second);

			for (size_t i = 0; i < N_HEROES; i++)
			{
				if (heroes[i]->get_x() == select_x and heroes[i]->get_y() == select_y)
				{
					//선택한 영웅 인덱스 저장
					selected_index = i;
					break;
				}
			}

			//이동 페이즈로 전환
			++gameplay.turn_action.second;
		}
	}
	//이동 페이즈
	else if (gameplay.turn_action.second == gameplay.MOVE)
	{
		//자신의 영웅 클릭시 리턴
		if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first) return;

		//영웅 기준으로 상하좌우 중 하나 클릭한 게 아니면 리턴
		if (!(click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) - 1) ||
			click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) + 1) ||
			click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y)) ||
			click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y))))
			return;

		//이동 중
		heroes[selected_index]->Set_Move(1);
		heroes[selected_index]->Move_Per_Frame(Index_To_Pos(click_index.first), Index_To_Pos(click_index.second));

		//이동 완료 했을 때
		if (heroes[selected_index]->Get_Move() == 2)
		{
			//타 플레이어 영웅 클릭했다면 해당 영웅 삭제
			if (heroes_pos[click_index.second][click_index.first] != 0)
			{
				//위치 정보로 상대 영웅 찾은 후 삭제
				for (size_t i = 0; i < N_HEROES; i++)
				{
					if (gameplay.turn_action.first == 1)
					{
						if (p2_heroes[i]->get_x() == Index_To_Pos(click_index.first) and
							p2_heroes[i]->get_y() == Index_To_Pos(click_index.second))
						{
							p2_heroes[i].reset();
							p2_heroes[i] = make_shared<C_None>(0, 0);
							break;
						}
					}
					else 	if (gameplay.turn_action.first == 2)
					{
						if (p1_heroes[i]->get_x() == Index_To_Pos(click_index.first) and
							p1_heroes[i]->get_y() == Index_To_Pos(click_index.second))
						{
							p1_heroes[i].reset();
							p1_heroes[i] = make_shared<C_None>(0, 0);
							break;
						}
					}
				}
			}


			++gameplay.turn_action.second;

			//보드 상 위치 정보 갱신
			heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
			heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;

			heroes[selected_index]->Set_Move(0);
			click_index = { -1,-1 };
		}
	}
	//능력 사용 페이즈
	else 	if (gameplay.turn_action.second == gameplay.SKILL)
	{
		if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first)
			for (size_t i = 0; i < N_HEROES; i++)
			{
				pair<int, int> tmp = { Pos_To_Index(heroes[i]->get_x()), Pos_To_Index(heroes[i]->get_y()) };
				if (click_index == tmp)
				{
					heroes[selected_index]->Set_Move(1);
					heroes[selected_index]->Use_Skill(*heroes[i]);

					if (heroes[selected_index]->Get_Move() == 0)
					{
						if(gameplay.turn_action.first == 1)
							gameplay.turn_action.first = 2;
						else
							gameplay.turn_action.first = 1;
						gameplay.turn_action.second = gameplay.SELECT;
					}
					break;
				}
			}
	}
}