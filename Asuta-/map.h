#pragma once
const int MapWidth = 5;
const int MapHeight = 5;

// �R�X�g�e�[�u��
const int CostTable1[MapHeight][MapWidth] =
{
	//0  1  2  3  4
	{ 3, 1, 1, 1, 3}, // 0
	{ 3, 3, 3, 3, 3}, // 1
	{ 3, 1, 1, 1, 3}, // 2
	{ 3, 1, 1, 1, 3}, // 3
	{ 3, 1, 1, 2, 3}, // 4
};
const int CostTable2[MapHeight][MapWidth] =
{
	//0  1  2  3  4
	{ 3, 1, 1, 1, 3}, // 0
	{ 3, 1, 1, 1, 3}, // 1
	{ 3, 1, 1, 1, 3}, // 2
	{ 3, 1, 1, 1, 3}, // 3
	{ 3, 1, 1, 2, 3}, // 4
};