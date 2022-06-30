#include "GameScene.h"
#include "TextureManager.h"
#include"AxisIndicator.h"
#include <cassert>
#include<cmath>
#include<random>
#include"affine.h"
#include"map.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete route_list;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("haikei.png");
	PlayerHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロダクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	/*for (int z = 0; z < 5; z++)
	{
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				worldTransform_[z][y][x].Initialize();
				worldTransform_[z][y][x].translation_ = { (float)x*2,(float)y*2,(float)z*2 };
				MyFanc::makeMatIdentity(worldTransform_[z][y][x].matWorld_);
				MyFanc::makeMatTrans(worldTransform_[z][y][x].matWorld_, worldTransform_[z][y][x].translation_);
				worldTransform_[z][y][x].TransferMatrix();
			}
		}
	}*/
	for (int y = 0; y < 3; y++)
	{
		for (int z = 0; z < 5; z++)
		{
			for (int x = 0; x < 5; x++)
			{
				if (map[y][z][x] == 1)
				{
					worldTransform_[y][z][x].Initialize();
					worldTransform_[y][z][x].translation_ = { (float)x * 2,(float)y * 2,(float)-z*2 };
					MyFanc::makeMatIdentity(worldTransform_[y][z][x].matWorld_);
					MyFanc::makeMatTrans(worldTransform_[y][z][x].matWorld_, worldTransform_[y][z][x].translation_);
					worldTransform_[y][z][x].TransferMatrix();
				}
			}
		}
	}
	Player.Initialize();
	route_list = new std::list<Cell>;
}

void GameScene::Update() {
	debugCamera_->Update();

	if (Asuta == false){
		time = clock();
		kousin(0, 0, 3, 1, 0, 1, route_list);
		Asuta = true;
	}
	else if (Asuta == true){
		if (clock() -time >= 1000){
			Cell player = route_list->front();
			route_list->pop_front();
			if (route_list->empty() == true) { Asuta = false; }
			Player.translation_ = { (float)player.X * 2,(float)player.Z* 2,(float)-player.Y * 2 };
			MyFanc::makeMatIdentity(Player.matWorld_);
			MyFanc::makeMatTrans(Player.matWorld_, Player.translation_);
			Player.TransferMatrix();
			time = clock();
		}
	}
			debugText_->SetPos(0, 0);
			debugText_->Printf("%f,%f,%f", Player.translation_.x, Player.translation_.y, Player.translation_.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	for (int y = 0; y < 3; y++)
	{
		for (int z = 0; z < 5; z++)
		{
			for (int x = 0; x < 5; x++)
			{
				if (map[y][z][x] == 1)
				{
					model_->Draw(worldTransform_[y][z][x], debugCamera_->GetViewProjection(), textureHandle_);
				}
			}
		}
	}
	model_->Draw(Player, debugCamera_->GetViewProjection(), PlayerHandle_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
