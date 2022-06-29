#pragma once

#include "WorldTransform.h"

namespace MyFanc
{
	//�P�ʍs��
	void makeMatIdentity(Matrix4& matrix);

	//�A�t�B���ϊ��g�k
	void makeMatScale(Matrix4& matrix, Vector3 scale);

	//�A�t�B���ϊ���]
	void makeMatRot(Matrix4& matrix, Vector3 rot);

	//�A�t�B���ϊ����s�ړ�
	void makeMatTrans(Matrix4& matrix, Vector3 Trans);
}

