#include "Camera.h"
#include "Input.h"
#include "Player.h"

#include <math.h>

CAMERA camera; // �J������錾

// ����������
void CAMERA::Initialize() {
	position = POSITION_0_90_180_270[1]; // ���W��ݒ�
	target = VGet(STAGE_WIDTH_CENTER, 0.0f, STAGE_WIDTH_CENTER); // �����_��ݒ�
}

// �J�����̏���
void CAMERA::Process() {

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_LEFT)
	{
		position = POSITION_0_90_180_270[2]; // ���W��ݒ�
	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_RIGHT)
	{
		position = POSITION_0_90_180_270[0]; // ���W��ݒ�
	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_UP)
	{
		position = POSITION_0_90_180_270[3]; // ���W��ݒ�
	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_DOWN)
	{
		position = POSITION_0_90_180_270[1]; // ���W��ݒ�
	}

	// �J�����̈ʒu�ƌ�����ݒ�
	SetCameraPositionAndTarget_UpVecY(position, target);
}

// �R���X�g���N�^
CAMERA::CAMERA() {

	// ����������
	Initialize();
}

// �f�X�g���N�^
CAMERA::~CAMERA() {

}