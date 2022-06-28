#include "DxLib.h"
#include"class.h"
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
	int playerx = 1;
	int playery = 0;
	int playerz = 1;
	int Enemyx = 0;
	int Enemyy = 0;
	int Enemyz = 3;
	std::list<Cell>* route_list=new std::list<Cell>;
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
		kousin(playerx, playery,playerz, Enemyx, Enemyy,Enemyz, route_list);

		// 描画処理
		for (int y = 0; y < MapHeight; y++)
		{
			for (int x = 0; x < MapWidth; x++)
			{
				switch (CostTable1[y][x])
				{
				case 0:
					DrawBox(x * 128, y * 128, x * 128 + 128, y * 128 + 128, GetColor(0, 0, 0), true);
					break;
				case 1:
					DrawBox(x * 128, y * 128, x * 128 + 128, y * 128 + 128, GetColor(255, 128, 0), true);
					break;
				case 2:
					DrawBox(x * 128, y * 128, x * 128 + 128, y * 128 + 128, GetColor(255, 255, 0), true);
					break;
				case 3:
					DrawBox(x * 128, y * 128, x * 128 + 128, y * 128 + 128, GetColor(255, 128, 255), true);
					break;
				default:
					DrawBox(x * 128, y * 128, x * 128 + 128, y * 128 + 128, GetColor(255, 255, 255), true);
					break;
				}
				if (playerx == x && playery == y)
				{
					DrawBox(playerx * 128, playery * 128, playerx * 128 + 128, playery * 128 + 128, GetColor(0, 255, 0), true);
				}
				if (Enemyx == x && Enemyy == y)
				{
					DrawBox(Enemyx * 128, Enemyy * 128, Enemyx * 128 + 128, Enemyy * 128 + 128, GetColor(0, 0, 255), true);
				}
				DrawFormatString(x * 128, y * 128, GetColor(0, 0, 0), "%d", CostTable1[y][x]);
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
		int i = 0;
		while (route_list->empty() == false)
		{
			Cell kiroku = route_list->front();
			route_list->pop_front();
			if (kiroku.Y==-1) {
				DrawFormatString(10, 0, GetColor(255, 255, 255), "経路は見つからなかった");
			}
			DrawFormatString(610, i * 15, GetColor(255, 255, 255), "%d,%d", kiroku.Y, kiroku.X);
			i++;

		}
		DrawFormatString(400, 0, GetColor(255, 255, 255), "goal %d,%d 緑", playery, playerx);
		DrawFormatString(400, 15, GetColor(255, 255, 255), "state  %d,%d 青", Enemyy, Enemyx);
		DrawFormatString(400, 30, GetColor(255, 255, 255), "1段目 オレンジ");
		DrawFormatString(400, 45, GetColor(255, 255, 255), "2段目　黄");
		DrawFormatString(400, 60, GetColor(255, 255, 255), "3段目　紫");
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
