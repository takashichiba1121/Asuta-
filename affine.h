#pragma once

#include "WorldTransform.h"

namespace MyFanc
{
	//単位行列
	void makeMatIdentity(Matrix4& matrix);

	//アフィン変換拡縮
	void makeMatScale(Matrix4& matrix, Vector3 scale);

	//アフィン変換回転
	void makeMatRot(Matrix4& matrix, Vector3 rot);

	//アフィン変換平行移動
	void makeMatTrans(Matrix4& matrix, Vector3 Trans);
}

