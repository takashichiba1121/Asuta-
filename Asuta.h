#pragma once
#include <memory.h>
#include <list>
#include <queue>
#include <vector>
#include <math.h>
#include"map.h"
const int Infinity = 100000;
// �폜����
enum EraseResult
{
	NotFound,		// ������
	Erased,			// �폜
	CouldntErased	// �폜�ł��Ȃ�
};
// Map�̃}�X��
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

	int X;		// ��
	int Y;		// �s
	int Z;		// ����
}Cell;

// �m�[�h
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

	Cell Position;						// �m�[�h���W
	std::vector<Node*> AdjacentNodes;	// �אڃm�[�h(��)
	float HeuristicCost;				// �q���[���X�e�B�b�N�R�X�g
	float TotalCost;					// �R�X�g(�q���[���X�e�B�b�N�R�X�g����)
}Node;

void kousin(int playerx, int playery, int playerz, int Enemyx, int Enemyy, int Enemyz, std::list<Cell>* route_list);
// �����\�[�g�p�֐�
bool Less(Node* a, Node* b);

// �Z���͈̓`�F�b�N�֐�
bool IsCellWithinTheRange(int x, int y);

// �m�[�h�̍쐬
void CreateMap(int playery, int Enemyy);

// �R�X�g������
void InitCost(int heuristic_cost, int total_cost);

// �q���[���X�e�B�b�N�R�X�g�v�Z(�m�[�h�ƃS�[���܂ł̋�����Ԃ��Ă���)
float CalculateHeuristic(const Node* node, const Node* Goal);

// �Z����r
bool IsEqualCell(const Cell& a, const Cell& b);

EraseResult EraseNode(std::list<Node*>& list, Node* new_node, float new_cost);

// �I�[�v�����X�g�ɒǉ�
bool AddAdjacentNode(std::list<Node*>& open_list, std::list<Node*>& close_list, Node* adjacent_node, float cost);

void AStar(Cell start, Cell goal,  std::list<Cell>* route_list,bool map);