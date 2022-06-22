#include "DxLib.h"
#include <memory.h>
#include <list>
#include <queue>
#include <vector>
#include <math.h>
const int MapWidth = 5;
const int MapHeight = 5;
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
		Y(-1)
	{
	}

	Cell(int x, int y)
	{
		X = x;
		Y = y;
	}

	int X;		// ��
	int Y;		// �s
}Cell;

// �m�[�h
typedef struct Node
{
	Node() :
		Node(0, 0)
	{
	}

	Node(int x, int y) :
		Position(x, y),
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
// �R�X�g�e�[�u��
int CostTable[MapHeight][MapWidth] =
{
	//0  1  2  3  4
	{ 1, 1, 2, 3, 2}, // 0
	{ 2, 0, 3, 4, 0}, // 1
	{ 3, 0, 0, 5, 6}, // 2
	{ 3, 2, 0, 0, 5}, // 3
	{ 0, 1, 2, 3, 4}, // 4
};

// �O���t
Node Map[MapHeight][MapWidth];

int kousin(int playerx, int playery, int Enemyx, int Enemyy, Cell* kiroku);
// �����\�[�g�p�֐�
bool Less(Node* a, Node* b);

// �Z���͈̓`�F�b�N�֐�
bool IsCellWithinTheRange(int x, int y);

// �m�[�h�̍쐬
void CreateMap();

// �R�X�g������
void InitCost(int heuristic_cost, int total_cost);

// �q���[���X�e�B�b�N�R�X�g�v�Z(�m�[�h�ƃS�[���܂ł̋�����Ԃ��Ă���)
float CalculateHeuristic(const Node* node, const Node* Goal);

// �Z����r
bool IsEqualCell(const Cell& a, const Cell& b);

EraseResult EraseNode(std::list<Node*>& list, Node* new_node, float new_cost);

// �I�[�v�����X�g�ɒǉ�
bool AddAdjacentNode(std::list<Node*>& open_list, std::list<Node*>& close_list, Node* adjacent_node, float cost);

int AStar(Cell start, Cell goal, Cell* kiroku);
// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "LE2B_15_�`�o�^�J�V";

// �E�B���h�E����
const int WIN_WIDTH = 640;

// �E�B���h�E�c��
const int WIN_HEIGHT = 640;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int playerx =1;
	int playery = 4;
	int Enemyx = 0;
	int Enemyy = 0;
	Cell kiroku[MapHeight * MapWidth];
	for (int i = 0; i < MapHeight * MapWidth; i++)
	{
		kiroku[i].X = 0;
		kiroku[i].Y = 0;
	}
	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 255; i++)
		{
			oldkeys[i] = keys[i];
		}
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
		/*if (keys[KEY_INPUT_LEFT] == true && oldkeys[KEY_INPUT_LEFT] == false)
		{
			playerx -= 1;
		}
		if (keys[KEY_INPUT_RIGHT] == true && oldkeys[KEY_INPUT_RIGHT] == false)
		{
			playerx += 1;
		}
		if (keys[KEY_INPUT_UP] == true && oldkeys[KEY_INPUT_UP] == false)
		{
			playery -= 1;
		}
		if (keys[KEY_INPUT_DOWN] == true && oldkeys[KEY_INPUT_DOWN] == false)
		{
			playery += 1;
		}*/
		int count=kousin(playerx,playery,Enemyx,Enemyy,kiroku);

		// �`�揈��
		for (int y = 0; y < MapHeight; y++)
		{
			for (int x = 0; x < MapWidth; x++)
			{
				if (CostTable[y][x] == 0) {
					DrawBox(x * 128, y * 128, x * 128 + 128, y * 128 + 128, GetColor(0, 0, 0), true);
				}
				if (CostTable[y][x] != 0) {
					DrawBox(x * 128, y * 128, x * 128 + 128, y * 128 + 128, GetColor(255, 128, 0), true);
				}
			}
		}
		for (int i = 0; i < WIN_WIDTH; i += 128)
		{
			DrawLine(i, 0, i, WIN_HEIGHT, GetColor(255, 255, 255));
		}
		for (int j = 0; j < WIN_HEIGHT; j += 128)
		{
			DrawLine(0, j, WIN_WIDTH + 128, j, GetColor(255, 255, 255));
		}
		DrawBox(playerx*128, playery*128, playerx*128 + 128, playery*128 + 128, GetColor(0, 255, 0), true);
		DrawBox(Enemyx * 128, Enemyy * 128, Enemyx * 128 + 128, Enemyy * 128 + 128, GetColor(0, 0, 255), true);
		if (kiroku[0].X == -1 && kiroku[0].Y == -1){
			DrawFormatString(10, 0, GetColor(255, 255, 255), "�o�H�͌�����Ȃ�����");
		}
		else {
			for (int i = 0; i < MapHeight * MapWidth; i++)
			{
				if (kiroku[i].X == -1 && kiroku[i].Y == -1)
				{
					break;
				}
				DrawFormatString(10, i * 15, GetColor(255, 255, 255), "%d,%d", kiroku[i].X, kiroku[i].Y);
			}
		}
		DrawFormatString(400, 0, GetColor(255, 255, 255), "state %d,%d ��", playerx,playery);
		DrawFormatString(400, 15, GetColor(255, 255, 255), "goal  %d,%d ��", Enemyx, Enemyy);
		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();
	// ����I��
	return 0;
}
int kousin(int playerx, int playery, int Enemyx, int Enemyy, Cell* kiroku)
{
	CreateMap();

	Cell start = Cell(Enemyx, Enemyx);
	Cell goal = Cell(playerx, playery);

	int Count = AStar(start, goal, kiroku);
	return Count;
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
void CreateMap()
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

			// �אڃm�[�h�̒ǉ�
			for (const Cell& cell : adjacent_cells)
			{
				if (IsCellWithinTheRange(cell.X, cell.Y) == true &&
					CostTable[cell.Y][cell.X] != 0&&
					abs(CostTable[cell.Y][cell.X]-CostTable[x][y]) <2)
				{
					Map[y][x].AdjacentNodes.push_back(&Map[cell.Y][cell.X]);
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

int AStar(Cell start, Cell goal, Cell* kiroku)
{
	std::list<Node*> open_list;
	std::list<Node*> close_list;

	/*const Node* start_node = &Map[start.Y][start.X];*/
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

			// �m�[�h�ԃR�X�g
			float edge_cost = CostTable[adjacent_node->Position.Y][adjacent_node->Position.X];
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

	// �o�H����
	std::list<Cell> route_list;

	// �S�[���Z�����畜������
	route_list.push_back(goal);
	int i = 0;
	while (route_list.empty() == false)
	{
		Cell route = route_list.front();
		// �X�^�[�g�Z���Ȃ�I��
		if (IsEqualCell(route, start) == true)
		{
			kiroku[i+1].X = -1;
			kiroku[i+1].Y = -1;
			break;
		}
		else
		{
			if (IsCellWithinTheRange(route.X, route.Y) == true)
			{
				// �ǉ�
				route_list.push_front(last_update_cells[route.Y][route.X]);
				kiroku[i].X = route.X;
				kiroku[i].Y = route.Y;
				i++;
			}
			else
			{
				kiroku[0].X = -1;
				kiroku[0].Y = -1;
				break;
			}
		}
	}
	return count;
}

