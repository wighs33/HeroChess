#include "stdafx.h"
#include "C_Board.h"

void C_Board::Render(HDC memdc)
{
	//Ÿ�� ������ ���� ��ĥ �ϱ�
	for (size_t h = 0; h < BOARD_H; h++)
		for (size_t w = 0; w < BOARD_W; w++)
		{
			//Ÿ���� �������� �Ѱ��ش�.
			HBRUSH hbrush = CreateSolidBrush(tiles_[h][w]->Get_Color());
			HBRUSH oldbrush = (HBRUSH)SelectObject(memdc, hbrush);
			Rectangle(memdc, Index_To_Pos(w), Index_To_Pos(h), Index_To_Pos(w + 1), Index_To_Pos(h + 1));
			SelectObject(memdc, oldbrush);
			DeleteObject(hbrush);
		}

	//���� �׸���
	HPEN hpen = CreatePen(PS_SOLID, 5, BLACK_BRUSH);
	HPEN oldbrush = (HPEN)SelectObject(memdc, hpen);
	for (int i = GRID_WH; i <= GRID_WH * 8; i += GRID_WH)
	{
		//����
		//������
		MoveToEx(memdc, i, GRID_WH, NULL);
		//����
		LineTo(memdc, i, GRID_WH * 11);
	}

	for (int i = GRID_WH; i <= GRID_WH * 11; i += GRID_WH)
	{
		//����
		//������
		MoveToEx(memdc, GRID_WH, i, NULL);
		//����
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

	SelectObject(memdc, oldbrush);
	DeleteObject(hpen);
}

void C_Board::Render_Heroes(HDC memdc)
{
	auto heroes = p1_heroes;
	auto opposing_heroes = p2_heroes;
	if (gameplay.turn_action.first == 2)
	{
		heroes = p2_heroes;
		opposing_heroes = p1_heroes;
	}

	bool is_moving = false;
	for (size_t i = 0; i < N_HEROES; i++)
	{

		//���� �� �׸���
		p1_heroes[i]->Render(memdc);
		p2_heroes[i]->Render(memdc);

		if (heroes[i]->Get_Move() == 1) is_moving = true;
	}

	//�� ���̶� �̵� ���� �� �� ǥ�� �ǳʶٱ�
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

	//�� ǥ��
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

			//��ġ�� ����ȭ�� �迭�� �ð����⵵ ���̱�
			if (heroes_pos[tmp_iy][tmp_ix] == gameplay.turn_action.first) continue;

			if (heroes_pos[tmp_iy][tmp_ix] == 0)
			{
				//�� ���� ����ġ�� ������ ǥ��, �Ʊ� ���� ǥ�� ����
				if(tmp_ix == 3 and tmp_iy == 0 and gameplay.turn_action.first == 2)
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, RED);
				else if (tmp_ix == 3 and tmp_iy == BOARD_H - 1 and gameplay.turn_action.first == 1)
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, RED);
				else if(!((tmp_ix == 3 and tmp_iy == 0) or (tmp_ix == 3 and tmp_iy == BOARD_H - 1)))
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, GREEN);
			}
			else
			{
				//������� ���� ǥ�� ���ֱ�
				if (selected_index == DEFENDER) continue;
				//���� ������ ���� ������� �ƴ� �� ���� �� ǥ��
				if (four_dir[dir_index] != make_pair<int, int>(opposing_heroes[DEFENDER]->get_x(), opposing_heroes[DEFENDER]->get_y()))
					Show_Color(memdc, four_dir[dir_index].first, four_dir[dir_index].second, RED);
			}
		}
	}
	else if (gameplay.turn_action.second == gameplay.SKILL)
	{
		if (selected_index == MAGICIAN)
		{
			for (size_t i = 1; i < N_HEROES; i++)
			{
				Show_Color(memdc, heroes[i]->get_x(), heroes[i]->get_y(), YELLOW);
			}
		}
		else if(selected_index == REAPER)
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

				if (heroes_pos[tmp_y][tmp_x] != gameplay.turn_action.first and heroes_pos[tmp_y][tmp_x] != 0)
				{
					Show_Color(memdc, eight_dir[dir_index].first, eight_dir[dir_index].second, YELLOW);
					++tmp_cnt;
				}
			}

			// ���� ��� ������ �ڵ����� �� ��ü
			if (tmp_cnt == 0)
			{
				Turn_Change();
			}
		}
		else if (selected_index == NINJA)
		{
			
		}
		else if (selected_index == GHOST)
		{
			//���� ���� �������� ������ �ڵ� �� ��ü
			//player1 �� ��
			if(gameplay.turn_action.first == 1)
				if (Pos_To_Index(select_y) != BOARD_H - 1)
				{
					Turn_Change();
					return;
				}

			//player2 �� ��
			if (gameplay.turn_action.first == 2)
				if (Pos_To_Index(select_y) != 0)
				{
					Turn_Change();
					return;
				}

			for (size_t i = 0; i < N_HEROES; i++)
			{
				Show_Color(memdc, opposing_heroes[i]->get_x(), opposing_heroes[i]->get_y(), YELLOW);
			}
		}
		else if (selected_index == WARRIOR)
		{
			//�ڵ�
		}
		else if (selected_index == DEFENDER)
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
		else if (selected_index == KNIGHT)
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

			// ���� ��� ������ �ڵ����� �� ��ü
			if (tmp_cnt == 0)
			{
				Turn_Change();
			}
		}
	}
}

void C_Board::Generate_Grid()
{
	for (size_t h = 0; h < BOARD_H; h++)
		for (size_t w = 0; w < BOARD_W; w++)
		{
			//���� 1/5Ȯ��
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

	//���� ��ġ�� ����
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
	//�簢���׸���  ��������
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
	//�ִϸ��̼� �� Ŭ�� ���� ����
	if (p1_heroes[selected_index]->Get_Move() == 1 or p2_heroes[selected_index]->Get_Move() == 1) return;

	click_index = { -1, -1 };

	//click_index�� Ŭ���� ĭ�� �ε��� �ֱ�
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
	//���� ���� �� Ŭ������ �ڵ� �ɷ� ���
	if (selected_index == 4 and gameplay.turn_action.second == gameplay.SKILL)
	{
		click_index = { -2,-2 };
	}

	//���� �� Ŭ���ϸ� ����
	if (click_index.first == -1 || click_index.second == -1) return;

	auto heroes = p1_heroes;
	auto opposing_heroes = p2_heroes;
	if (gameplay.turn_action.first == 2)
	{
		heroes = p2_heroes;
		opposing_heroes = p1_heroes;
	}


	//���� ������
	if (gameplay.turn_action.second == gameplay.SELECT)
	{
		if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first)
		{
			//Ŭ�� ��ġ ����
			select_x = Index_To_Pos(click_index.first);
			select_y = Index_To_Pos(click_index.second);

			for (size_t i = 0; i < N_HEROES; i++)
			{
				if (heroes[i]->get_x() == select_x and heroes[i]->get_y() == select_y)
				{
					if (heroes[i]->Is_Bound() == true) return;

					//������ ���� �ε��� ����
					selected_index = i;
					break;
				}
			}

			//�̵� ������� ��ȯ
			++gameplay.turn_action.second;
		}
	}
	//�̵� ������
	else if (gameplay.turn_action.second == gameplay.MOVE)
	{
		//�ڽ��� ���� Ŭ���� ����
		if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first) return;

		//���� �������� �����¿� �� �ϳ� Ŭ���� �� �ƴϸ� ����
		if (!(click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) - 1) ||
			click_index == make_pair<int, int>(Pos_To_Index(select_x), Pos_To_Index(select_y) + 1) ||
			click_index == make_pair<int, int>(Pos_To_Index(select_x) - 1, Pos_To_Index(select_y)) ||
			click_index == make_pair<int, int>(Pos_To_Index(select_x) + 1, Pos_To_Index(select_y))))
			return;

		//������� ������� ���� �Ұ�
		if (click_index == make_pair<int, int>(Pos_To_Index(opposing_heroes[DEFENDER]->get_x()), Pos_To_Index(opposing_heroes[DEFENDER]->get_y()))) return;

		//������� ��� ���� �Ұ�
		if (selected_index == DEFENDER and heroes_pos[click_index.second][click_index.first] != 0) return;

		//�Ʊ� �� Ŭ�� ��Ȱ��ȭ, ��� �� Ŭ�� �� ��� ������ ���� �� ���� ������
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


		//�̵� ��
		heroes[selected_index]->Move_Per_Frame(Index_To_Pos(click_index.first), Index_To_Pos(click_index.second));

		//�̵� �Ϸ� ���� ��
		if (heroes[selected_index]->Get_Move() == 2)
		{
			//Ÿ �÷��̾� ���� Ŭ���ߴٸ� �ش� ���� ����
			if (heroes_pos[click_index.second][click_index.first] != 0)
			{
				//��ġ ������ ��� ���� ã�� �� ����
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
					//else if�� �� �� ��ȣ�� ���ؼ� ��� if���� �����ϴ��� ��Ȯ�ϰ� ��������
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

			//���� �� ��ġ ���� ����
			heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
			heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;

			heroes[selected_index]->Set_Move(0);

			//���� ��ġ �� ����
			heroes[selected_index]->Before_x(select_x);
			heroes[selected_index]->Before_y(select_y);

			//���� ��ġ �� ����
			select_x = Index_To_Pos(click_index.first);
			select_y = Index_To_Pos(click_index.second);

			click_index = { -1,-1 };
		}
	}
	//�ɷ� ��� ������
	else 	if (gameplay.turn_action.second == gameplay.SKILL)
	{
		//������
		if (selected_index == MAGICIAN)
		{
			//���� ��� : �Ʊ�
			//�Ʊ� �ƴ� �� �� ��ü �� ����
			if (heroes_pos[click_index.second][click_index.first] != gameplay.turn_action.first)
			{
				Turn_Change();
				return;
			}

			//Ŭ���� ���� ã��
			for (size_t i = 0; i < N_HEROES; i++)
			{
				pair<int, int> tmp = { Pos_To_Index(heroes[i]->get_x()), Pos_To_Index(heroes[i]->get_y()) };
				if (click_index == tmp)
				{
					//��ų ���
					heroes[selected_index]->Use_Skill(heroes[i]);

					//�� ��ü
					if (heroes[selected_index]->Get_Move() == 0)
					{
						Turn_Change();
					}
					break;
				}
			}
		}
		//���
		else if (selected_index == REAPER)
		{
			//���� ��� : ���
			if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first or
				heroes_pos[click_index.second][click_index.first] == 0)
			{
				Turn_Change();
				return;
			}

			//���� : �� ĭ �ݰ�
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
			
			//Ŭ���� ���� ã��
			for (size_t i = 0; i < N_HEROES; i++)
			{
				pair<int, int> tmp = { Pos_To_Index(opposing_heroes[i]->get_x()), Pos_To_Index(opposing_heroes[i]->get_y()) };
				if (click_index == tmp)
				{
					//��ų ���
					heroes[selected_index]->Use_Skill(opposing_heroes[i]);

					//�� ��ü
					if (heroes[selected_index]->Get_Move() == 0)
					{
						heroes_pos[click_index.second][click_index.first] = 0;
						Turn_Change();
					}
					break;
				}
			}
		}
		//����
		else if (selected_index == NINJA)
		{
			Turn_Change();
		}
		//��Ʈ
		else if (selected_index == GHOST)
		{
			//���� ��� : ���
			if (heroes_pos[click_index.second][click_index.first] == gameplay.turn_action.first or
				heroes_pos[click_index.second][click_index.first] == 0)
			{
				Turn_Change();
				return;
			}

			//������ ����
			auto& target_hero = Find_Hero_By_Index(((gameplay.turn_action.first - 1) ^ 1) + 1, click_index.first, click_index.second);

			//��ų ���
			heroes[selected_index]->Use_Skill(target_hero);

			//�̵� ���� �� ����
			if (heroes[selected_index]->Get_Move() == 1) return;

			int tmp_iy;
			if (gameplay.turn_action.first == 1)
				tmp_iy = 1;
			else
				tmp_iy = BOARD_H - 2;

			//�ʱ� �ڸ��� ���� ���� �� ����
			auto& tmp_hero = Find_Hero_By_Index(ALL, 3, tmp_iy);
			if (tmp_hero)
			{
				tmp_hero.reset();
				tmp_hero = make_shared<C_None>(0, 0);
			}

			//�ʱ� �ڸ��� ���ư���
			heroes[selected_index]->set_x(Index_To_Pos(3));
			heroes[selected_index]->set_y(Index_To_Pos(tmp_iy));
			heroes_pos[tmp_iy][3] = gameplay.turn_action.first;

			heroes_pos[click_index.second][click_index.first] = 0;
			heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
			Turn_Change();
		}
		//����
		else if (selected_index == WARRIOR)
		{
			for (size_t i = 0; i < N_HEROES; i++)
			{
				opposing_heroes[i]->Is_Bound(false);
			}
			//���� : �� ĭ �ݰ�
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
					//��ų ��� : �� ĭ �ݰ� ��ο��� ����
					target_hero->Is_Bound(true);
				}
			}
			Turn_Change();
		}
		//�����
		else if (selected_index == DEFENDER)
		{
			for (int i = Pos_To_Index(select_x) + 1; i < BOARD_W; ++i)
			{
				//������ ���θ����� �̵� ���� ���� ������
				if (heroes_pos[Pos_To_Index(select_y)][i] != 0) break;

				if (click_index != make_pair<int, int>((int)i, Pos_To_Index(select_y))) continue;

				//�̵� ��
				heroes[DEFENDER]->Move_Per_Frame(Index_To_Pos((int)i), select_y);

				//�̵� �Ϸ� ���� ��
				if (heroes[DEFENDER]->Get_Move() == 2)
				{
					heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
					heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
					heroes[DEFENDER]->Set_Move(0);
					Turn_Change();
					return;
				}
			}

			for (int i = Pos_To_Index(select_x) - 1; i >= 0; --i)
			{
				if (heroes_pos[Pos_To_Index(select_y)][i] != 0) break;

				if (click_index != make_pair<int, int>((int)i, Pos_To_Index(select_y))) continue;

				//�̵� ��
				heroes[DEFENDER]->Move_Per_Frame(Index_To_Pos((int)i), select_y);

				//�̵� �Ϸ� ���� ��
				if (heroes[DEFENDER]->Get_Move() == 2)
				{
					heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
					heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
					heroes[DEFENDER]->Set_Move(0);
					Turn_Change();
					return;
				}
			}

			for (int i = Pos_To_Index(select_y) + 1; i < BOARD_H; ++i)
			{
				if (heroes_pos[i][Pos_To_Index(select_x)] != 0) break;

				if (click_index != make_pair<int, int>(Pos_To_Index(select_x), (int)i)) continue;

				//�̵� ��
				heroes[DEFENDER]->Move_Per_Frame(select_x, Index_To_Pos((int)i));

				//�̵� �Ϸ� ���� ��
				if (heroes[DEFENDER]->Get_Move() == 2)
				{
					heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
					heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
					heroes[DEFENDER]->Set_Move(0);
					Turn_Change();
					return;
				}
			}

			for (int i = Pos_To_Index(select_y) - 1; i >= 0; --i)
			{
				if (heroes_pos[i][Pos_To_Index(select_x)] != 0) break;

				if (click_index != make_pair<int, int>(Pos_To_Index(select_x), (int)i)) continue;

				//�̵� ��
				heroes[DEFENDER]->Move_Per_Frame(select_x, Index_To_Pos((int)i));

				//�̵� �Ϸ� ���� ��
				if (heroes[DEFENDER]->Get_Move() == 2)
				{
					heroes_pos[click_index.second][click_index.first] = gameplay.turn_action.first;
					heroes_pos[Pos_To_Index(select_y)][Pos_To_Index(select_x)] = 0;
					heroes[DEFENDER]->Set_Move(0);
					Turn_Change();
					return;
				}
			}
			//��� ǥ�ð� �ƴ� ĭ Ŭ�� �� �� ��ü
			if (heroes[DEFENDER]->Get_Move() == 0)
			{
				Turn_Change();
				return;
			}
		}
		//���
		else if (selected_index == KNIGHT)
		{
			//���� ��� : �Ʊ�
			if (heroes_pos[click_index.second][click_index.first] != gameplay.turn_action.first)
			{
				Turn_Change();
				return;
			}

			//���� : �� ĭ �ݰ�
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

			//Ŭ���� ���� ã��
			if (heroes[save]->Get_Move() == 0)
				for (size_t i = 0; i < N_HEROES; i++)
				{
					pair<int, int> tmp = { Pos_To_Index(heroes[i]->get_x()), Pos_To_Index(heroes[i]->get_y()) };
					if (click_index == tmp)
					{
						dest_x = heroes[KNIGHT]->get_x() - heroes[KNIGHT]->Before_x() + heroes[i]->get_x();
						dest_y = heroes[KNIGHT]->get_y() - heroes[KNIGHT]->Before_y() + heroes[i]->get_y();
						save = i;
						if (Find_Hero_By_Index(gameplay.turn_action.first, Pos_To_Index(dest_x), Pos_To_Index(dest_y))) return;
						break;
					}
				}

			//�̵� ��
			heroes[save]->Move_Per_Frame(dest_x, dest_y);

			//�̵� �Ϸ� ���� ��
			if (heroes[save]->Get_Move() == 2)
			{
				//Ŭ�� ��ġ : �ɷ� ���� ���
				heroes_pos[click_index.second][click_index.first] = 0;
				//�ɷ� ���� �� ��ġ
				heroes_pos[Pos_To_Index(dest_y)][Pos_To_Index(dest_x)] = gameplay.turn_action.first;
				heroes[save]->Set_Move(0);
				Turn_Change();
				return;
			}
		}

	
	}
}

void C_Board::Turn_Change()
{
	gameplay.turn_action.first = ((gameplay.turn_action.first - 1) ^ 1) + 1;
	gameplay.turn_action.second = gameplay.SELECT;
	click_index = { -1,-1 };
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

	shared_ptr<C_Hero> tmp = nullptr;
	return tmp;
}
