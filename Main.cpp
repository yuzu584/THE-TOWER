#include "DxLib.h"
#include "Stage.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(true);

	// ���C�u�����̏�����
	if (DxLib_Init() == -1) return -1;

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// �J�����̃N���b�s���O������ݒ�
	SetCameraNearFar(0.1f, 300.0f);

	// �w�i�̐F��ݒ�
	SetBackgroundColor(128, 128, 128);

	player = new Player();

	stage.Initialize(0);

	// ESC�L�[��������邩�A�E�C���h�E��������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ���݂̃J�E���g���擾����
		int time = GetNowCount();

		// ��ʂ��N���A
		ClearDrawScreen();

		// ���͏���
		input.Process();

		// �v���C���[�̏���
		player->Process();

		// �J�����̏���
		camera.Process();

		// �X�e�[�W��`��
		stage.Render();

		// �v���C���[���f���̕`��
		int m = player->GetModelHandle();
		MV1DrawModel(m);

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();

		// �P�V�~���b(��b�ԂU�O�t���[�����������̂P�t���[��������̌o�ߎ���)
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