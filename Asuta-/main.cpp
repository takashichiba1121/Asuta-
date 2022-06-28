#include "DxLib.h"
#include"class.h"
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
	int playerx = 1;
	int playery = 0;
	int playerz = 1;
	int Enemyx = 0;
	int Enemyy = 0;
	int Enemyz = 3;
	std::list<Cell>* route_list=new std::list<Cell>;
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
		kousin(playerx, playery,playerz, Enemyx, Enemyy,Enemyz, route_list);

		// �`�揈��
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
				DrawFormatString(10, 0, GetColor(255, 255, 255), "�o�H�͌�����Ȃ�����");
			}
			DrawFormatString(610, i * 15, GetColor(255, 255, 255), "%d,%d", kiroku.Y, kiroku.X);
			i++;

		}
		DrawFormatString(400, 0, GetColor(255, 255, 255), "goal %d,%d ��", playery, playerx);
		DrawFormatString(400, 15, GetColor(255, 255, 255), "state  %d,%d ��", Enemyy, Enemyx);
		DrawFormatString(400, 30, GetColor(255, 255, 255), "1�i�� �I�����W");
		DrawFormatString(400, 45, GetColor(255, 255, 255), "2�i�ځ@��");
		DrawFormatString(400, 60, GetColor(255, 255, 255), "3�i�ځ@��");
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
