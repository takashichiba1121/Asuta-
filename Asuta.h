#pragma once
#include <memory.h>
#include <list>
#include <queue>
#include <vector>
#include <math.h>
#include"map.h"
const int Infinity = 100000;
// 削除結果
enum EraseResult
{
	NotFound,		// 未発見
	Erased,			// 削除
	CouldntErased	// 削除できない
};
// Mapのマス目
typedef struct Cell
{
	Cell() :
		X(-1),
		Y(-1),
		Z(-1)
	{
	}

	Cell(int x, int y, int z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	int X;		// 列
	int Y;		// 行
	int Z;		// 高さ
}Cell;

// ノード
typedef struct Node
{
	Node() :
		Node(0, 0, 0)
	{
	}

	Node(int x, int y, int z) :
		Position(x, y, z),
		HeuristicCost(Infinity),
		TotalCost(0)
	{
		AdjacentNodes.clear();
	}

	Cell Position;						// ノード座標
	std::vector<Node*> AdjacentNodes;	// 隣接ノード(辺)
	float HeuristicCost;				// ヒューリスティックコスト
	float TotalCost;					// コスト(ヒューリスティックコスト込み)
}Node;

void kousin(int playerx, int playery, int playerz, int Enemyx, int Enemyy, int Enemyz, std::list<Cell>* route_list);
// 昇順ソート用関数
bool Less(Node* a, Node* b);

// セル範囲チェック関数
bool IsCellWithinTheRange(int x, int y);

// ノードの作成
void CreateMap(int playery, int Enemyy);

// コスト初期化
void InitCost(int heuristic_cost, int total_cost);

// ヒューリスティックコスト計算(ノードとゴールまでの距離を返している)
float CalculateHeuristic(const Node* node, const Node* Goal);

// セル比較
bool IsEqualCell(const Cell& a, const Cell& b);

EraseResult EraseNode(std::list<Node*>& list, Node* new_node, float new_cost);

// オープンリストに追加
bool AddAdjacentNode(std::list<Node*>& open_list, std::list<Node*>& close_list, Node* adjacent_node, float cost);

void AStar(Cell start, Cell goal,  std::list<Cell>* route_list,bool map);