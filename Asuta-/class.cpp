#include"class.h"

Node Map[MapHeight][MapWidth];

void kousin(int playerx, int playery,int playerz , int Enemyx, int Enemyy, int Enemyz, std::list<Cell>* route_list)
{
	Cell tyuukan = { -1, -1 };
	if ( playerz == 1 && Enemyz == 3 ||
		playerz == 3 && Enemyz == 1)
	{
		for (int y = 0; y < MapHeight; y++)
		{
			for (int x = 0; x < MapWidth; x++)
			{
				if (CostTable1[y][x] == 2)
				{
					if (tyuukan.X == -1 && tyuukan.Y == -1)
					{
						tyuukan.X = x;
						tyuukan.Y = y;
					}
					else if ((tyuukan.X - playerx) + (tyuukan.Y - playery) > (x - playerx) + (y - playery))
					{
						tyuukan.X = x;
						tyuukan.Y = y;
					}
				}
			}
		}
		Cell start = Cell(Enemyx, Enemyy);
		Cell goal = tyuukan;

		AStar(start,Enemyz, goal,2, route_list);

		start = tyuukan;
		goal = Cell(playerx, playery);

		AStar(start,2, goal,playerz, route_list);
	}
	else
	{
		Cell start = Cell(Enemyx, Enemyy);
		Cell goal = Cell(playerx, playery);

		AStar(start,Enemyz, goal,playerz, route_list);
	}
}
// �����\�[�g�p�֐�
bool Less(Node* a, Node* b)
{
	if (a->TotalCost < b->TotalCost)
	{
		return true;
	}

	return false;
}

// �Z���͈̓`�F�b�N�֐�
bool IsCellWithinTheRange(int x, int y)
{
	if (x >= 0 &&
		x < MapWidth &&
		y >= 0 &&
		y < MapHeight)
	{
		return true;
	}

	return false;
}

// �m�[�h�̍쐬
void CreateMap(int playerz, int Enemyz)
{

	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			Map[y][x].Position.X = x;
			Map[y][x].Position.Y = y;

			Cell adjacent_cells[] =
			{
				Cell(x, y - 1),
				Cell(x - 1, y),
				Cell(x + 1, y),
				Cell(x, y + 1),
			};
			Map[y][x].AdjacentNodes.clear();
			if (playerz == 1 && Enemyz == 1 ||
				playerz == 2 && Enemyz == 1 ||
				playerz == 1 && Enemyz == 2)
			{
				// �אڃm�[�h�̒ǉ�
				for (const Cell& cell : adjacent_cells)
				{
					if (IsCellWithinTheRange(cell.X, cell.Y) == true &&
						CostTable2[cell.Y][cell.X] != 0 &&
						abs(CostTable2[cell.Y][cell.X] - CostTable2[y][x]) < 2)
					{
						Map[y][x].AdjacentNodes.push_back(&Map[cell.Y][cell.X]);
					}
				}
			}
			else
			{
				// �אڃm�[�h�̒ǉ�
				for (const Cell& cell : adjacent_cells)
				{
					if (IsCellWithinTheRange(cell.X, cell.Y) == true &&
						CostTable1[cell.Y][cell.X] != 0 &&
						abs(CostTable1[cell.Y][cell.X] - CostTable1[y][x]) < 2)
					{
						Map[y][x].AdjacentNodes.push_back(&Map[cell.Y][cell.X]);
					}
				}
			}
		}
	}
}

// �R�X�g������
void InitCost(int heuristic_cost, int total_cost)
{
	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			Map[y][x].HeuristicCost = heuristic_cost;
			Map[y][x].TotalCost = total_cost;
		}
	}
}

// �q���[���X�e�B�b�N�R�X�g�v�Z(�m�[�h�ƃS�[���܂ł̋�����Ԃ��Ă���)
float CalculateHeuristic(const Node* node, const Node* Goal)
{
	float x = fabsf(Goal->Position.X - node->Position.X);
	float y = fabsf(Goal->Position.Y - node->Position.Y);

	return sqrtf(x * x + y * y);
}

// �Z����r
bool IsEqualCell(const Cell& a, const Cell& b)
{
	if (a.X == b.X &&
		a.Y == b.Y)
	{
		return true;
	}

	return false;
}

EraseResult EraseNode(std::list<Node*>& list, Node* new_node, float new_cost)
{
	// �N���[�Y���X�g�`�F�b�N
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		// �m�[�h�Ɠ����Z�������邩���ׂ�
		if (IsEqualCell(new_node->Position, (*itr)->Position) == true)
		{
			// �R�X�g�̔�r
			if (new_cost < (*itr)->TotalCost)
			{
				list.erase(itr);
				return EraseResult::Erased;
			}
			else
			{
				return EraseResult::CouldntErased;
			}
		}
	}

	return EraseResult::NotFound;
}

// �I�[�v�����X�g�ɒǉ�
bool AddAdjacentNode(std::list<Node*>& open_list, std::list<Node*>& close_list, Node* adjacent_node, float cost)
{
	bool can_update = true;

	std::list<Node*> node_list[] =
	{
		close_list,
		open_list
	};

	for (std::list<Node*>& list : node_list)
	{
		// ���X�g�ɓ����m�[�h�������ă��X�g�̕��̃R�X�g�������Ȃ�폜
		if (EraseNode(list, adjacent_node, cost) == EraseResult::CouldntErased)
		{
			can_update = false;
		}
	}

	if (can_update == true)
	{
		open_list.push_back(adjacent_node);
		return true;
	}

	return false;
}

void AStar(Cell start, int startz, Cell goal, int goalz, std::list<Cell>* route_list)
{
	CreateMap(startz, goalz);
	std::list<Node*> open_list;
	std::list<Node*> close_list;

	const Node* start_node = &Map[start.Y][start.X];
	const Node* goal_node = &Map[goal.Y][goal.X];

	// �X�V�����m�[�h�ʒu�ۑ��p
	Cell last_update_cells[MapHeight][MapWidth];

	// �O���t�̏�����
	InitCost(Infinity, 0);

	// �X�^�[�g�m�[�h�̎w��
	open_list.push_back(&Map[start.Y][start.X]);

	// �o�H�T����
	int count = 0;

	// �I�[�v�����X�g���Ȃ��Ȃ�܂ŉ�
	while (open_list.empty() == false)
	{
		count++;

		Node* search_node = (*open_list.begin());
		// �T�����X�g���珜�O
		open_list.erase(open_list.begin());

		// �S�[���ɓ��B������I���
		if (IsEqualCell(search_node->Position, goal) == true)
		{
			// �N���[�Y���X�g�ɍŌ�̃m�[�h��ǉ�����
			close_list.push_back(search_node);
			break;
		}

		for (Node* adjacent_node : search_node->AdjacentNodes)
		{
			// �v�Z���s���Ă��Ȃ������ꍇ�����v�Z
			if (adjacent_node->HeuristicCost == Infinity)
			{
				// �q���[���X�e�B�N�X�R�X�g�v�Z
				adjacent_node->HeuristicCost = CalculateHeuristic(adjacent_node, goal_node);
			}
			float edge_cost;
			// �m�[�h�ԃR�X�g
			if (startz == 1 && goalz == 1 ||
				startz == 2 && goalz == 1 ||
				startz == 1 && goalz == 2)
			{
				edge_cost = CostTable2[adjacent_node->Position.Y][adjacent_node->Position.X];
			}
			else
			{
				edge_cost = CostTable1[adjacent_node->Position.Y][adjacent_node->Position.X];
			}
			// �擾�m�[�h�̃g�[�^���R�X�g
			float node_cost = search_node->TotalCost;
			/*
				�g�[�^���R�X�g�Z�o
					�m�[�h�ԃR�X�g + �q���[���X�e�B�b�N�R�X�g + �擾�m�[�h�̃g�[�^���R�X�g
			*/
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			// �m�[�h�ǉ�
			if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost) == true)
			{
				// �g�[�^���R�X�g���X�V
				adjacent_node->TotalCost = total_cost;

				if (adjacent_node->Position.Y == 0 && adjacent_node->Position.X == 2)
				{
					int xx = 0;
					xx = 100;
				}

				// �o�H���X�V�����Z����ۑ�
				last_update_cells[adjacent_node->Position.Y][adjacent_node->Position.X] = search_node->Position;
			}
		}

		bool is_add_close = true;

		// �N���[�Y���X�g�`�F�b�N
		for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
		{
			// �m�[�h�Ɠ����Z�������邩���ׂ�
			if (IsEqualCell(search_node->Position, (*itr)->Position) == true)
			{
				is_add_close = false;
				break;
			}
		}

		// �����m�[�h�����������̂Œǉ�
		if (is_add_close == true)
		{
			// ���̃m�[�h�̒T���I��
			close_list.push_back(search_node);
		}

		// �����\�[�g
		open_list.sort(Less);
	}

	// �S�[���Z�����畜������
	std::list<Cell> routelist;
	routelist.push_front(goal);
	while (routelist.empty() == false)
	{
		Cell route = routelist.front();
		// �X�^�[�g�Z���Ȃ�I��
		if (IsEqualCell(route, start) == true)
		{
			break;
		}
		else
		{
			if (IsCellWithinTheRange(route.X, route.Y) == true)
			{
				// �ǉ�
				routelist.push_front(last_update_cells[route.Y][route.X]);
			}
			else
			{
				break;
			}
		}
	}
	if (route_list->empty() == true)
	{
		while (routelist.empty() == false)
		{
			route_list->push_back(routelist.back());
			routelist.pop_back();
		}
	}
	else
	{
		while (routelist.empty() == false)
		{
			route_list->push_front(routelist.front());
			routelist.pop_front();
		}
	}
}

