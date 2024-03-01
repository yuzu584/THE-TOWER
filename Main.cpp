#include "DxLib.h"
#include "Stage.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "StageCreate.h"
#include <windows.h>
#include <algorithm>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(true);

	// ��ʃT�C�Y��ݒ�
	SetGraphMode(800, 600, 16);

	// ���C�u�����̏�����
	if (DxLib_Init() == -1) return -1;

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// �J�����̃N���b�s���O������ݒ�
	SetCameraNearFar(0.1f, 300.0f);

	// �w�i�̐F��ݒ�
	SetBackgroundColor(128, 128, 128);

	stage.Initialize(0);
	stage.Create();

	player.Initialize();

	// �W�����C�g�̕�����Y���̃}�C�i�X�����ɂ���
	SetLightDirection(VGet(0.5f, -1.0f, 0.5f));

	// �������~���b�P�ʂŐ���
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	srand(tm.wMilliseconds);

	// ESC�L�[��������邩�A�E�C���h�E��������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ���݂̃J�E���g���擾����
		int time = GetNowCount();

		// ��ʂ��N���A
		ClearDrawScreen();
		
		// ���͏���
		input.Process();

		// �J�����̏���
		camera.Process();

		// �X�e�[�W�̏���
		stage.Process();

		// �v���C���[�̏���
		player.Process();

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();

		// 17�~���b(��b��60�t���[������������1�t���[��������̌o�ߎ���)
		// �o�߂���܂ł����ő҂�
		while (GetNowCount() - time < 17)
		{
		}
	}

	// ���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�I��
	return 0;
}