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
	wsprintf(lpOut2, _T("turn_action : %d %d"), gameplay.turn_action.first, gameplay.turn_action.second);
	TextOut(memdc, 20, 20, lpOut2, lstrlen(lpOut2));

	static TCHAR lpOut3[100];
	wsprintf(lpOut3, _T("click_index : %d %d"), click_index.first, click_index.second);
	TextOut(memdc, 20, 40, lpOut3, lstrlen(lpOut3));


	static TCHAR skill_description[500];

	switch (selected_index)
	{
	case MAGICIAN:
		wsprintf(skill_description, _T(" 마법사 능력 : 아군 영웅 한 명과 위치를 교체한다. "));
		break;
	case REAPER:
		wsprintf(skill_description, _T(" 사신 능력 : 주위에 있는 적군 영웅 한 명을 제거한다. "));
		break;
	case NINJA:
		wsprintf(skill_description, _T(" 전사의 능력은 카피할 수 없다. (지속 능력은 카피 불가) "));
		TextOut(memdc, 0, WIN_H - 80, skill_description, lstrlen(skill_description));
		wsprintf(skill_description, _T(" 닌자 능력 : 주위에 있는 영웅 한 명의 능력을 카피 후 자신의 턴동안 사용한다. "));
		break;
	case GHOST:
		wsprintf(skill_description, _T(" 초기 위치에 영웅이 존재할 때 능력 사용 후 그 영웅은 제거된다. "));
		TextOut(memdc, 0, WIN_H - 80, skill_description, lstrlen(skill_description));
		wsprintf(skill_description, _T(" 고스트 능력 : 적군 진영 끝에 도달 시 한 명을 제거하고 초기 위치로 돌아온다. "));
		break;
	case WARRIOR:
		wsprintf(skill_description, _T(" 전사 능력 : 주위에 있는 적군 영웅 모두를 무력화한다. "));
		break;
	case DEFENDER:
		wsprintf(skill_description, _T(" 디펜더는 공격 행동을 할 수 없고 공격 대상이 될 수 없다. "));
		TextOut(memdc, 0, WIN_H - 80, skill_description, lstrlen(skill_description));
		wsprintf(skill_description, _T(" 디펜더 능력 : 가로 또는 세로 방향으로 이동한다. (장애물이 있으면 막힌다.)"));
		break;
	case KNIGHT:
		wsprintf(skill_description, _T(" 기사 능력 : 주위에 있는 아군 영웅 한 명을 기사의 이동 방향으로 이동시킨다. "));
		break;
	default:
		wsprintf(skill_description, _T(""));
	}
	TextOut(memdc, 0, WIN_H - 100, skill_description, lstrlen(skill_description));

	SelectObject(memdc, oldbrush);
	DeleteObject(hpen);
}

void C_Board::Render_Heroes(HDC memdc)
{
	heroes = p1_heroes;
	opposing_heroes = p2_heroes;
	if (gameplay.turn_action.first == 2)
	{
		heroes = p2_heroes;
		opposing_heroes = p1_heroes;
	}

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

	for (size_t i = 0; i < N_HEROES; i++)
	{
		if (heroes[i]->Is_Bound())
			Show_Color(memdc, heroes[i]->get_x(), heroes[i]->get_y(), GRAY);
		if (opposing_heroes[i]->Is_Bound())
			Show_Color(memdc, opposing_heroes[i]->get_x(), opposing_heroes[i]->get_y(), GRAY);
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
			int tmp_ix = Pos_To_Index(four_dir[dir_index].first);
			int tmp_iy = Pos_To_Index(four_dir[dir_index].second);

			//위치를 간략화한 배열로 시간복잡도 줄이기
			if (heroes_pos[tmp_iy][tmp_ix] == gameplay.turn_action.first) continue;

			if (heroes_pos[tmp_iy][tmp_ix] == 0)
			{
				//적 성에 마주치면 빨간색 표시, 아군 성은 표시 안함
				if(tmp_ix == 3 and tmp_iy == 0 and gameplay.turn_action.first == 2)
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, RED);
				else if (tmp_ix == 3 and tmp_iy == BOARD_H - 1 and gameplay.turn_action.first == 1)
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, RED);
				else if(!((tmp_ix == 3 and tmp_iy == 0) or (tmp_ix == 3 and tmp_iy == BOARD_H - 1)))
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, GREEN);
			}
			else
			{
				//디펜더는 공격 표시 없애기
				if (selected_index == DEFENDER) continue;
				//공격 가능한 적이 디펜더가 아닐 시 빨간 색 표시
				if (four_dir[dir_index] != make_pair<int, int>(opposing_heroes[DEFENDER]->get_x(), opposing_heroes[DEFENDER]->get_y()))
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, RED);
			}
		}
	}
	else if (gameplay.turn_action.second == gameplay.SKILL)
	{
		if (selected_index == MAGICIAN)			Magician_Skill_Range(memdc, MAGICIAN);
		else if (selected_index == REAPER)			Reaper_And_Ninja_Skill_Range(memdc);
		else if (selected_index == NINJA)
		{
			//카피 후
			if (skill_copy_index == MAGICIAN)			Magician_Skill_Range(memdc, NINJA);
			else if (skill_copy_index == REAPER)		Reaper_And_Ninja_Skill_Range(memdc);
			else if (skill_copy_index == GHOST)		Ghost_Skill_Range(memdc);
			else if (skill_copy_index == WARRIOR)	Turn_Change(); //카피 불가
			else if (skill_copy_index == DEFENDER)	Defender_Skill_Range(memdc);
			else if (skill_copy_index == KNIGHT)		Knight_Skill_Range(memdc);
			//카피 전
			else															Reaper_And_Ninja_Skill_Range(memdc);
		}
		else if (selected_index == GHOST)			Ghost_Skill_Range(memdc);
		else if (selected_index == WARRIOR);	//자동
		else if (selected_index == DEFENDER)	Defender_Skill_Range(memdc);
		else if (selected_index == KNIGHT)		Knight_Skill_Range(memdc);
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
	//애니메이션 중 클릭 갱신 막기
	if (selected_index != -1)
		if (p1_heroes[selected_index]->Get_Move() == 1 or p2_heroes[selected_index]->Get_Move() == 1) return;

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
	//전사 선택 시 클릭없이 자동 능력 사용
	if (selected_index == WARRIOR and gameplay.turn_action.second == gameplay.SKILL)
	{
		click_index = { -2,-2 };
	}

	//보드 밖 클릭하면 리턴
	if (click_index.first == -1 || click_index.second == -1) return;

	heroes = p1_heroes;
	opposing_heroes = p2_heroes;
	if (gameplay.turn_action.first == 2)
	{
		heroes = p2_heroes;
		opposing_heroes = p1_heroes;
	}


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
					if (heroes[i]->Is_Bound() == true) return;

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

		//디펜더를 대상으로 공격 불가
		if (click_index == make_pair<int, int>(Pos_To_Index(opposing_heroes[DEFENDER]->get_x()), Pos_To_Index(opposing_heroes[DEFENDER]->get_y()))) return;

		//디펜더는 상대 공격 불가
		if (selected_index == DEFENDER and heroes_pos[click_index.second][click_index.first] != 0) return;

		//아군 성 클릭 비활성화, 상대 성 클릭 시 상대 라이프 감소 후 다음 페이즈
		if (gameplay.turn_action.first == 1)
		{
			if (click_index == make_pair<int, int>(3, 0)) return;
			if (click_index == make_pair<int, int>(3, BOARD_H - 1))
			{
				--lifes.second;
				
				++gameplay.turn_action.second;
				click_index = { -1,-1 };
				return;
			}
		}
		else
		{
			if (click_index == make_pair<int, int>(3, BOARD_H - 1)) return;
			if (click_index == make_pair<int, int>(3, 0))
			{
				--lifes.first;

				++gameplay.turn_action.second;
				click_index = { -1,-1 };
				return;
			}
		}


		//이동 중
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
					//else if를 쓸 때 괄호를 통해서 어느 if문과 대응하는지 명확하게 구분짓자
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

			//이전 위치 값 저장
			heroes[selected_index]->Before_x(select_x);
			heroes[selected_index]->Before_y(select_y);

			//현재 위치 값 갱신
			select_x = Index_To_Pos(click_index.first);
			select_y = Index_To_Pos(click_index.second);

			click_index = { -1,-1 };
		}
	}
	//능력 사용 페이즈
	else 	if (gameplay.turn_action.second == gameplay.SKILL)
	{
		//마법사
		if (selected_index == MAGICIAN)			Magician_Skill();
		//사신
		else if (selected_index == REAPER)			Reaper_Skill();
		//닌자
		else if (selected_index == NINJA)			Ninja_Skill();
		//고스트
		else if (selected_index == GHOST)			Ghost_Skill();
		//전사
		else if (selected_index == WARRIOR)		Warrior_Skill();
		//디펜더
		else if (selected_index == DEFENDER)	Defender_Skill();
		//기사
		else if (selected_index == KNIGHT)		Knight_Skill();
	}
}

void C_Board::Turn_Change()
{
	gameplay.turn_action.first = ((gameplay.turn_action.first - 1) ^ 1) + 1;
	gameplay.turn_action.second = gameplay.SELECT;
	click_index = { -1,-1 };

	if (selected_index == NINJA)
	{
		skill_copy_index = -1;
		is_select_copy = false;
	}

	heroes[selected_index]->Before_x(0);
	heroes[selected_index]->Before_y(0);
}

shared_ptr<C_Hero>& C_Board::Find_Hero_By_Index(int who, int ix, int iy)
{
	for (size_t i = 0; i < N_HEROES; i++)
	{
		if (who == 1 and p1_heroes[i]->get_x() == Index_To_Pos(ix) and p1_heroes[i]->get_y() == Index_To_Pos(iy))
			return p1_heroes[i];
		else if (who == 2 and p2_heroes[i]->get_x() == Index_To_Pos(ix) and p2_heroes[i]->get_y() == Index_To_Pos(iy))
			return p2_heroes[i];
		else if (who == 3)
		{
			if (p1_heroes[i]->get_x() == Index_To_Pos(ix) and p1_heroes[i]->get_y() == Index_To_Pos(iy))
				return p1_heroes[i];
			else if (p2_heroes[i]->get_x() == Index_To_Pos(ix) and p2_heroes[i]->get_y() == Index_To_Pos(iy))
				return p2_heroes[i];
		}
	}

	return null_hero;
}

void C_Board::Magician_Skill_Range(HDC memdc, int except)
{
	for (size_t i = 0; i < N_HEROES; i++)
	{
		if (i == except) continue;
		Show_Color(memdc, heroes[i]->get_x(), heroes[i]->get_y(), YELLOW);
	}
}

void C_Board::Reaper_And_Ninja_Skill_Range(HDC memdc)
{
	pair<int, int> eight_dir[8];
	eight_dir[0] = { select_x, select_y - GRID_WH };
	eight_dir[1] = { select_x, select_y + GRID_WH };
	eight_dir[2] = { select_x - GRID_WH, select_y };
	eight_dir[3] = { select_x + GRID_WH, select_y };
	eight_dir[4] = { select_x - GRID_WH, select_y - GRID_WH };
	eight_dir[5] = { select_x + GRID_WH, select_y - GRID_WH };
	eight_dir[6] = { select_x - GRID_WH, select_y + GRID_WH };
	eight_dir[7] = { select_x + GRID_WH, select_y + GRID_WH };

	int tmp_cnt = 0;
	for (size_t dir_index = 0; dir_index < 8; dir_index++)
	{
		int tmp_x = Pos_To_Index(eight_dir[dir_index].first);
		int tmp_y = Pos_To_Index(eight_dir[dir_index].second);

		if (heroes_pos[tmp_y][tmp_x] != 0)
		{
			if ((selected_index == REAPER or skill_copy_index == REAPER) and heroes_pos[tmp_y][tmp_x] != gameplay.turn_action.first)
			{
				Show_Color(memdc, eight_dir[dir_index].first, eight_dir[dir_index].second, YELLOW);
				++tmp_cnt;
			}
			else if (selected_index == NINJA and skill_copy_index != REAPER)
			{
				Show_Color(memdc, eight_dir[dir_index].first, eight_dir[dir_index].second, PURPLE);
				++tmp_cnt;
			}
		}
	}

	// 적용 대상 없으면 자동으로 턴 교체
	if (tmp_cnt == 0)
	{
		Turn_Change();
	}
}

void C_Board::Ghost_Skill_Range(HDC memdc)
{
	//적진 끝에 도달하지 않으면 자동 턴 교체
	int last_line = gameplay.turn_action.first == 1 ? (BOARD_H - 1) : 0;
	if (Pos_To_Index(select_y) != last_line)
	{
		Turn_Change();
		return;
	}

	for (size_t i = 0; i < N_HEROES; i++)
	{
		Show_Color(memdc, opposing_heroes[i]->get_x(), opposing_heroes[i]->get_y(), YELLOW);
	}
}

void C_Board::Defender_Skill_Range(HDC memdc)
{
	for (int i = Pos_To_Index(select_x) + 1; i < BOARD_W; ++i)
	{
		if (heroes_pos[Pos_To_Index(select_y)][i] != 0) break;
		Show_Color(memdc, Index_To_Pos(i), select_y, YELLOW);
	}

	for (int i = Pos_To_Index(select_x) - 1; i >= 0; --i)
	{
		if (heroes_pos[Pos_To_Index(select_y)][i] != 0) break;
		Show_Color(memdc, Index_To_Pos(i), select_y, YELLOW);
	}

	for (int i = Pos_To_Index(select_y) + 1; i < BOARD_H; ++i)
	{
		if (heroes_pos[i][Pos_To_Index(select_x)] != 0) break;
		Show_Color(memdc, select_x, Index_To_Pos(i), YELLOW);
	}

	for (int i = Pos_To_Index(select_y) - 1; i >= 0; --i)
	{
		if (heroes_pos[i][Pos_To_Index(select_x)] != 0) break;
		Show_Color(memdc, select_x, Index_To_Pos(i), YELLOW);
	}
}

void C_Board::Knight_Skill_Range(HDC memdc)
{
	pair<int, int> eight_dir[8];
	eight_dir[0] = { select_x, select_y - GRID_WH };
	eight_dir[1] = { select_x, select_y + GRID_WH };
	eight_dir[2] = { select_x - GRID_WH, select_y };
	eight_dir[3] = { select_x + GRID_WH, select_y };
	eight_dir[4] = { select_x - GRID_WH, select_y - GRID_WH };
	eight_dir[5] = { select_x + GRID_WH, select_y - GRID_WH };
	eight_dir[6] = { select_x - GRID_WH, select_y + GRID_WH };
	eight_dir[7] = { select_x + GRID_WH, select_y + GRID_WH };

	int tmp_cnt = 0;
	for (size_t dir_index = 0; dir_index < 8; dir_index++)
	{
		int tmp_x = Pos_To_Index(eight_dir[dir_index].first);
		int tmp_y = Pos_To_Index(eight_dir[dir_index].second);

		if (heroes_pos[tmp_y][tmp_x] == gameplay.turn_action.first)
		{
			Show_Color(memdc, eight_dir[dir_index].first, eight_dir[dir_index].second, YELLOW);
			++tmp_cnt;
		}
	}

	// 적용 대상 없으면 자동으로 턴 교체
	if (tmp_cnt == 0)
	{
		Turn_Change();
	}
}

void C_Board::Magician_Skill()
{
	//적용 대상 : 아군
	//아군 아닐 때 턴 교체 후 리턴
	if (heroes_pos[click_index.second][click_index.first] != gameplay.turn_action.first)
	{
		Turn_Change();
		return;
	}

	//클릭한 영웅 찾기
	for (size_t i = 0; i < N_HEROES; i++)
	{
		pair<int, int> tmp = { Pos_To_Index(heroes[i]->get_x()), Pos_To_Index(heroes[i]->get_y()) };
		if (click_index == tmp)
		{
			heroes[selected_index]->Animation();

			if (heroes[selected_index]->Get_Move() == 1) return;

			//마법사 능력 : 위치 체인지
			int tmp_x = heroes[selected_index]->get_x();
			int tmp_y = heroes[selected_index]->get_y();

			heroes[selected_index]->set_x(heroes[i]->get_x());
			heroes[selected_index]->set_y(heroes[i]->get_y());

			heroes[i]->set_x(tmp_x);
			heroes[i]->set_y(tmp_y);

			//턴 교체
			if (heroes[selected_index]->Get_Move() == 0)
			{
				Turn_Change();
				return;
			}
			break;
		}
	}
}

void C_Board::Reaper_Skill()
{
	//적용 대상 : 상대
	if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first or
		heroes_pos[click_index.second][click_index.first] == 0)
	{
		Turn_Change();
		return;
	}

	//범위 : 한 칸 반경
	if (!(click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) - 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) + 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y)) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y)) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y) + 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y) - 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y) + 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y) - 1)
		))
	{
		Turn_Change();
		return;
	}

	//클릭한 영웅 찾기
	for (size_t i = 0; i < N_HEROES; i++)
	{
		pair<int, int> tmp = { Pos_To_Index(opposing_heroes[i]->get_x()), Pos_To_Index(opposing_heroes[i]->get_y()) };
		if (click_index == tmp)
		{
			//애니메이션 재생
			heroes[selected_index]->Animation();

			if (heroes[selected_index]->Get_Move() == 1) return;

			//사신 능력 : 주위 영웅 한명 제거
			opposing_heroes[i].reset();
			opposing_heroes[i] = make_shared<C_None>(0, 0);

			//턴 교체
			if (heroes[selected_index]->Get_Move() == 0)
			{
				heroes_pos[click_index.second][click_index.first] = 0;
				Turn_Change();
				return;
			}
			break;
		}
	}
}

void C_Board::Ninja_Skill()
{
	//능력페이즈 때 한번만 초기화
	if (!is_select_copy)
	{
		skill_copy_index = -1;

		//클릭 위치 저장
		int tmp_x = Index_To_Pos(click_index.first);
		int tmp_y = Index_To_Pos(click_index.second);

		for (size_t i = 0; i < N_HEROES; i++)
		{
			if (heroes[i]->get_x() == tmp_x and heroes[i]->get_y() == tmp_y)
			{
				//선택한 영웅 인덱스 저장
				skill_copy_index = i;
				break;
			}
			else if (opposing_heroes[i]->get_x() == tmp_x and opposing_heroes[i]->get_y() == tmp_y)
			{
				//선택한 영웅 인덱스 저장
				skill_copy_index = i;
				break;
			}
		}

		//선택하지 않으면 턴 교체
		if (skill_copy_index == -1)
		{
			Turn_Change();
			return;
		}

		is_select_copy = true;
		//클릭 다시 하기
		click_index = { -1,-1 };
		return;
	}

	//카피할 영웅 선택했을 시
	switch (skill_copy_index)
	{
	case MAGICIAN:
		Magician_Skill();
		break;
	case REAPER:
		Reaper_Skill();
		break;
	case GHOST:
		Ghost_Skill();
		break;
	case WARRIOR:
		break;
	case DEFENDER:
		Defender_Skill();
		break;
	case KNIGHT:
		Knight_Skill();
		break;
	}
}

void C_Board::Ghost_Skill()
{
	//적용 대상 : 상대
	if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first or
		heroes_pos[click_index.second][click_index.first] == 0)
	{
		Turn_Change();
		return;
	}

	//제거할 영웅
	auto& target_hero = Find_Hero_By_Index(((gameplay.turn_action.first - 1) ^ 1) + 1, click_index.first, click_index.second);

	//애니메이션 재생
	heroes[selected_index]->Animation();

	if (heroes[selected_index]->Get_Move() == 1) return;

	//고스트 능력 : 맨 끝 도달 시 주위 영웅 한명 제거
	target_hero.reset();
	target_hero = make_shared<C_None>(0, 0);

	int tmp_iy;
	if (gameplay.turn_action.first == 1)
		tmp_iy = 1;
	else
		tmp_iy = BOARD_H - 2;

	//초기 자리에 영웅 있을 때 삭제
	auto& tmp_hero = Find_Hero_By_Index(ALL, 3, tmp_iy);
	if (tmp_hero)
	{
		tmp_hero.reset();
		tmp_hero = make_shared<C_None>(0, 0);
	}

	//초기 자리로 돌아가기
	heroes[selected_index]->set_x(Index_To_Pos(3));
	heroes[selected_index]->set_y(Index_To_Pos(tmp_iy));
	heroes_pos[tmp_iy][3] = gameplay.turn_action.first;

	heroes_pos[click_index.second][click_index.first] = 0;
	heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
	Turn_Change();
}

void C_Board::Warrior_Skill()
{
	for (size_t i = 0; i < N_HEROES; i++)
	{
		opposing_heroes[i]->Is_Bound(false);
	}
	//범위 : 한 칸 반경
	pair<int, int> eight_dir[8];
	eight_dir[0] = { heroes[WARRIOR]->get_x(), heroes[WARRIOR]->get_y() - GRID_WH };
	eight_dir[1] = { heroes[WARRIOR]->get_x(), heroes[WARRIOR]->get_y() + GRID_WH };
	eight_dir[2] = { heroes[WARRIOR]->get_x() - GRID_WH, heroes[WARRIOR]->get_y() };
	eight_dir[3] = { heroes[WARRIOR]->get_x() + GRID_WH, heroes[WARRIOR]->get_y() };
	eight_dir[4] = { heroes[WARRIOR]->get_x() - GRID_WH, heroes[WARRIOR]->get_y() - GRID_WH };
	eight_dir[5] = { heroes[WARRIOR]->get_x() + GRID_WH, heroes[WARRIOR]->get_y() - GRID_WH };
	eight_dir[6] = { heroes[WARRIOR]->get_x() - GRID_WH, heroes[WARRIOR]->get_y() + GRID_WH };
	eight_dir[7] = { heroes[WARRIOR]->get_x() + GRID_WH, heroes[WARRIOR]->get_y() + GRID_WH };

	for (size_t dir_index = 0; dir_index < 8; dir_index++)
	{
		int tmp_x = Pos_To_Index(eight_dir[dir_index].first);
		int tmp_y = Pos_To_Index(eight_dir[dir_index].second);

		int opposing = ((gameplay.turn_action.first - 1) ^ 1) + 1;
		if (heroes_pos[tmp_y][tmp_x] == opposing)
		{
			auto& target_hero = Find_Hero_By_Index(opposing, tmp_x, tmp_y);
			//스킬 사용 : 한 칸 반경 모두에게 적용
			target_hero->Is_Bound(true);
		}
	}
	Turn_Change();
}

void C_Board::Defender_Skill()
{
	for (int i = Pos_To_Index(select_x) + 1; i < BOARD_W; ++i)
	{
		//영웅이 가로막으면 이동 가능 범위 좁혀짐
		if (heroes_pos[Pos_To_Index(select_y)][i] != 0) break;

		if (click_index != make_pair<int, int>((int)i, Pos_To_Index(select_y))) continue;

		//이동 중
		heroes[selected_index]->Move_Per_Frame(Index_To_Pos((int)i), select_y);

		//이동 완료 했을 때
		if (heroes[selected_index]->Get_Move() == 2)
		{
			heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
			heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
			heroes[selected_index]->Set_Move(0);
			Turn_Change();
			return;
		}
	}

	for (int i = Pos_To_Index(select_x) - 1; i >= 0; --i)
	{
		if (heroes_pos[Pos_To_Index(select_y)][i] != 0) break;

		if (click_index != make_pair<int, int>((int)i, Pos_To_Index(select_y))) continue;

		//이동 중
		heroes[selected_index]->Move_Per_Frame(Index_To_Pos((int)i), select_y);

		//이동 완료 했을 때
		if (heroes[selected_index]->Get_Move() == 2)
		{
			heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
			heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
			heroes[selected_index]->Set_Move(0);
			Turn_Change();
			return;
		}
	}

	for (int i = Pos_To_Index(select_y) + 1; i < BOARD_H; ++i)
	{
		if (heroes_pos[i][Pos_To_Index(select_x)] != 0) break;

		if (click_index != make_pair<int, int>(Pos_To_Index(select_x), (int)i)) continue;

		//이동 중
		heroes[selected_index]->Move_Per_Frame(select_x, Index_To_Pos((int)i));

		//이동 완료 했을 때
		if (heroes[selected_index]->Get_Move() == 2)
		{
			heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
			heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
			heroes[selected_index]->Set_Move(0);
			Turn_Change();
			return;
		}
	}

	for (int i = Pos_To_Index(select_y) - 1; i >= 0; --i)
	{
		if (heroes_pos[i][Pos_To_Index(select_x)] != 0) break;

		if (click_index != make_pair<int, int>(Pos_To_Index(select_x), (int)i)) continue;

		//이동 중
		heroes[selected_index]->Move_Per_Frame(select_x, Index_To_Pos((int)i));

		//이동 완료 했을 때
		if (heroes[selected_index]->Get_Move() == 2)
		{
			heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
			heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
			heroes[selected_index]->Set_Move(0);
			Turn_Change();
			return;
		}
	}
	//노란 표시가 아닌 칸 클릭 시 턴 교체
	if (heroes[selected_index]->Get_Move() == 0)
	{
		Turn_Change();
		return;
	}
}

void  C_Board::Knight_Skill()
{
	//적용 대상 : 아군
	if (heroes_pos[click_index.second][click_index.first] != gameplay.turn_action.first)
	{
		Turn_Change();
		return;
	}

	//범위 : 한 칸 반경
	if (!(click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) - 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) + 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y)) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y)) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y) + 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y) - 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y) + 1) ||
		click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y) - 1)
		))
	{
		Turn_Change();
		return;
	}

	static int save = 0;
	static int dest_x = 0;
	static int dest_y = 0;

	//클릭한 영웅 찾기
	if (heroes[save]->Get_Move() == 0)
		for (size_t i = 0; i < N_HEROES; i++)
		{
			pair<int, int> tmp = { Pos_To_Index(heroes[i]->get_x()), Pos_To_Index(heroes[i]->get_y()) };
			if (click_index == tmp)
			{
				dest_x = heroes[selected_index]->get_x() - heroes[selected_index]->Before_x() + heroes[i]->get_x();
				dest_y = heroes[selected_index]->get_y() - heroes[selected_index]->Before_y() + heroes[i]->get_y();
				save = i;
				//목적지에 아군 영웅 있을 시 이동 불가
				if (Find_Hero_By_Index(gameplay.turn_action.first, Pos_To_Index(dest_x), Pos_To_Index(dest_y))) return;
				//목적지에 적군 영웅 있을 시 공격
				auto& enemy = Find_Hero_By_Index(((gameplay.turn_action.first - 1) ^ 1) + 1, Pos_To_Index(dest_x), Pos_To_Index(dest_y));
				if (enemy)
				{
					enemy.reset();
					enemy = make_shared<C_None>(0, 0);
				}
				//목적지가 보드 밖일 때 이동 불가
				if (!(0 <= Pos_To_Index(dest_x) and Pos_To_Index(dest_x) < BOARD_W and 0 <= Pos_To_Index(dest_y) and Pos_To_Index(dest_y) < BOARD_H)) return;
				break;
			}
		}

	//이동 중
	heroes[save]->Move_Per_Frame(dest_x, dest_y);

	//이동 완료 했을 때
	if (heroes[save]->Get_Move() == 2)
	{
		//클릭 위치 : 능력 적용 대상
		heroes_pos[click_index.second][click_index.first] = 0;
		//능력 적용 후 위치
		heroes_pos[Pos_To_Index(dest_y)][Pos_To_Index(dest_x)] = gameplay.turn_action.first;
		heroes[save]->Set_Move(0);
		Turn_Change();
		return;
	}
}