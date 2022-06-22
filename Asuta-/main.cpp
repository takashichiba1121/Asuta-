#include "DxLib.h"
#include <memory.h>
#include <list>
#include <queue>
#include <vector>
#include <math.h>
const int MapWidth = 5;
const int MapHeight = 5;
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
		Y(-1)
	{
	}

	Cell(int x, int y)
	{
		X = x;
		Y = y;
	}

	int X;		// 列
	int Y;		// 行
}Cell;

// ノード
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

	Cell Position;						// ノード座標
	std::vector<Node*> AdjacentNodes;	// 隣接ノード(辺)
	float HeuristicCost;				// ヒューリスティックコスト
	float TotalCost;					// コスト(ヒューリスティックコスト込み)
}Node;
// コストテーブル
int CostTable[MapHeight][MapWidth] =
{
	//0  1  2  3  4
	{ 1, 1, 2, 3, 2}, // 0
	{ 2, 0, 3, 4, 0}, // 1
	{ 3, 0, 0, 5, 6}, // 2
	{ 3, 2, 0, 0, 5}, // 3
	{ 0, 1, 2, 3, 4}, // 4
};

// グラフ
Node Map[MapHeight][MapWidth];

int kousin(int playerx, int playery, int Enemyx, int Enemyy, Cell* kiroku);
// 昇順ソート用関数
bool Less(Node* a, Node* b);

// セル範囲チェック関数
bool IsCellWithinTheRange(int x, int y);

// ノードの作成
void CreateMap();

// コスト初期化
void InitCost(int heuristic_cost, int total_cost);

// ヒューリスティックコスト計算(ノードとゴールまでの距離を返している)
float CalculateHeuristic(const Node* node, const Node* Goal);

// セル比較
bool IsEqualCell(const Cell& a, const Cell& b);

EraseResult EraseNode(std::list<Node*>& list, Node* new_node, float new_cost);

// オープンリストに追加
bool AddAdjacentNode(std::list<Node*>& open_list, std::list<Node*>& close_list, Node* adjacent_node, float cost);

int AStar(Cell start, Cell goal, Cell* kiroku);
// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LE2B_15_チバタカシ";

// ウィンドウ横幅
const int WIN_WIDTH = 640;

// ウィンドウ縦幅
const int WIN_HEIGHT = 640;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み

	// ゲームループで使う変数の宣言
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
	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 255; i++)
		{
			oldkeys[i] = keys[i];
		}
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
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

		// 描画処理
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
			DrawFormatString(10, 0, GetColor(255, 255, 255), "経路は見つからなかった");
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
		DrawFormatString(400, 0, GetColor(255, 255, 255), "state %d,%d 緑", playerx,playery);
		DrawFormatString(400, 15, GetColor(255, 255, 255), "goal  %d,%d 青", Enemyx, Enemyy);
		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();
	// 正常終了
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

			// 隣接ノードの追加
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

// コスト初期化
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

// ヒューリスティックコスト計算(ノードとゴールまでの距離を返している)
float CalculateHeuristic(const Node* node, const Node* Goal)
{
	float x = fabsf(Goal->Position.X - node->Position.X);
	float y = fabsf(Goal->Position.Y - node->Position.Y);

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

int AStar(Cell start, Cell goal, Cell* kiroku)
{
	std::list<Node*> open_list;
	std::list<Node*> close_list;

	/*const Node* start_node = &Map[start.Y][start.X];*/
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

			// ノード間コスト
			float edge_cost = CostTable[adjacent_node->Position.Y][adjacent_node->Position.X];
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

	// 経路復元
	std::list<Cell> route_list;

	// ゴールセルから復元する
	route_list.push_back(goal);
	int i = 0;
	while (route_list.empty() == false)
	{
		Cell route = route_list.front();
		// スタートセルなら終了
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
				// 追加
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

