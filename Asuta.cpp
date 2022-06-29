#include"Asuta.h"
#include"map.h"

Node Map[MapHeight][MapWidth];

void kousin(int playerx, int playery, int playerz, int Enemyx, int Enemyy, int Enemyz, std::list<Cell>* route_list)
{
	Cell tyuukan = { -1, -1,2 };
	if (playerz == 1 && Enemyz == 3 ||
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
		Cell start = Cell(Enemyx, Enemyy,Enemyz);
		Cell goal = tyuukan;
		if (start.Z == 3)
		{
			AStar(start, goal, route_list,0);
			start = tyuukan;
			goal = Cell(playerx, playery, playerz);

			AStar(start, goal, route_list,1);
		}
		else
		{
			AStar(start, goal, route_list, 1);
			start = tyuukan;
			goal = Cell(playerx, playery, playerz);

			AStar(start, goal, route_list,0);
		}
	}
	else
	{
		Cell start = Cell(Enemyx, Enemyy, Enemyz);
		Cell goal = Cell(playerx, playery, playerz);
		
		if (start.Z == 3)
		{
			AStar(start, goal, route_list,0);
		}
		else
		{
			AStar(start, goal, route_list, 1);
		}
	}
}
// 昇順ソート用関数
bool Less(Node* a, Node* b)
{
	if (a->TotalCost < b->TotalCost)
	{
		return true;
	}

	return false;
}

// セル範囲チェック関数
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

// ノードの作成
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
				Cell(x, y - 1,0),
				Cell(x - 1, y,0),
				Cell(x + 1, y,0),
				Cell(x, y + 1,0),
			};
			Map[y][x].AdjacentNodes.clear();
			if (playerz == 1 && Enemyz == 1 ||
				playerz == 2 && Enemyz == 1 ||
				playerz == 1 && Enemyz == 2)
			{
				// 隣接ノードの追加
				for (const Cell& cell : adjacent_cells)
				{
					if (IsCellWithinTheRange(cell.X, cell.Y) == true &&
						CostTable2[cell.Y][cell.X] != 0 &&
						abs(CostTable2[cell.Y][cell.X] - CostTable2[y][x]) < 2)
					{
						Map[y][x].AdjacentNodes.push_back(&Map[cell.Y][cell.X]);
						Map[y][x].Position.Z = CostTable2[y][x];
					}
				}
			}
			else
			{
				// 隣接ノードの追加
				for (const Cell& cell : adjacent_cells)
				{
					if (IsCellWithinTheRange(cell.X, cell.Y) == true &&
						CostTable1[cell.Y][cell.X] != 0 &&
						abs(CostTable1[cell.Y][cell.X] - CostTable1[y][x]) < 2)
					{
						Map[y][x].AdjacentNodes.push_back(&Map[cell.Y][cell.X]);
						Map[y][x].Position.Z = CostTable1[y][x];
					}
				}
			}
		}
	}
}

// コスト初期化
void InitCost(int heuristic_cost, int total_cost)
{
	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			Map[y][x].HeuristicCost = float(heuristic_cost);
			Map[y][x].TotalCost = float(total_cost);
		}
	}
}

// ヒューリスティックコスト計算(ノードとゴールまでの距離を返している)
float CalculateHeuristic(const Node* node, const Node* Goal)
{
	float x = fabsf(float(Goal->Position.X - node->Position.X));
	float y = fabsf(float(Goal->Position.Y - node->Position.Y));

	return sqrtf(x * x + y * y);
}

// セル比較
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
	// クローズリストチェック
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		// ノードと同じセルがあるか調べる
		if (IsEqualCell(new_node->Position, (*itr)->Position) == true)
		{
			// コストの比較
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

// オープンリストに追加
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
		// リストに同じノードがあってリストの方のコストが高いなら削除
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

void AStar(Cell start, Cell goal,  std::list<Cell>* route_list,bool map)
{
	CreateMap(start.Z, goal.Z);
	std::list<Node*> open_list;
	std::list<Node*> close_list;

	const Node* start_node = &Map[start.Y][start.X];
	const Node* goal_node = &Map[goal.Y][goal.X];

	// 更新したノード位置保存用
	Cell last_update_cells[MapHeight][MapWidth];

	// グラフの初期化
	InitCost(Infinity, 0);

	// スタートノードの指定
	open_list.push_back(&Map[start.Y][start.X]);

	// 経路探索回数
	int count = 0;

	// オープンリストがなくなるまで回す
	while (open_list.empty() == false)
	{
		count++;

		Node* search_node = (*open_list.begin());
		// 探索リストから除外
		open_list.erase(open_list.begin());

		// ゴールに到達したら終わり
		if (IsEqualCell(search_node->Position, goal) == true)
		{
			// クローズリストに最後のノードを追加する
			close_list.push_back(search_node);
			break;
		}

		for (Node* adjacent_node : search_node->AdjacentNodes)
		{
			// 計算を行っていなかった場合だけ計算
			if (adjacent_node->HeuristicCost == Infinity)
			{
				// ヒューリスティクスコスト計算
				adjacent_node->HeuristicCost = CalculateHeuristic(adjacent_node, goal_node);
			}
			float edge_cost;
			// ノード間コスト
			if (start.Z == 1 && goal.Z == 1 ||
				start.Z == 2 && goal.Z == 1 ||
				start.Z == 1 && goal.Z == 2)
			{
				edge_cost = float(CostTable2[adjacent_node->Position.Y][adjacent_node->Position.X]);
			}
			else
			{
				edge_cost = float(CostTable1[adjacent_node->Position.Y][adjacent_node->Position.X]);
			}
			// 取得ノードのトータルコスト
			float node_cost = search_node->TotalCost;
			/*
				トータルコスト算出
					ノード間コスト + ヒューリスティックコスト + 取得ノードのトータルコスト
			*/
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			// ノード追加
			if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost) == true)
			{
				// トータルコストを更新
				adjacent_node->TotalCost = total_cost;

				if (adjacent_node->Position.Y == 0 && adjacent_node->Position.X == 2)
				{
					int xx = 0;
					xx = 100;
				}

				// 経路を更新したセルを保存
				last_update_cells[adjacent_node->Position.Y][adjacent_node->Position.X] = search_node->Position;
			}
		}

		bool is_add_close = true;

		// クローズリストチェック
		for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
		{
			// ノードと同じセルがあるか調べる
			if (IsEqualCell(search_node->Position, (*itr)->Position) == true)
			{
				is_add_close = false;
				break;
			}
		}

		// 同じノードが無かったので追加
		if (is_add_close == true)
		{
			// このノードの探索終了
			close_list.push_back(search_node);
		}

		// 昇順ソート
		open_list.sort(Less);
	}

	// ゴールセルから復元する
	std::list<Cell> routelist;
	routelist.push_front(goal);
	while (routelist.empty() == false)
	{
		Cell route = routelist.front();
		// スタートセルなら終了
		if (IsEqualCell(route, start) == true)
		{
			break;
		}
		else
		{
			if (IsCellWithinTheRange(route.X, route.Y) == true)
			{
				// 追加
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
