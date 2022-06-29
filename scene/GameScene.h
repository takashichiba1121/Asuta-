﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#include<time.h>
#include<list>
#include"Asuta.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t PlayerHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;



	//ワールドトランスフォーム
	WorldTransform worldTransform_[3][5][5];
	//ビュープロダクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//円周率
	float PI = 3.1415927;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	bool Asuta=false;

	clock_t time=0;

	std::list<Cell>* route_list;

	WorldTransform Player;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
