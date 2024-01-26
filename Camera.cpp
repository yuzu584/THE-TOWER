#include "Camera.h"
#include "Input.h"

#include <math.h>

Camera camera; // �J������錾

// ����������
void Camera::Initialize() {
	position = POSITION_0_90_180_270[0]; // ���W��ݒ�
	target = VGet(5.0f, 0.0f, 5.0f);     // �����_��ݒ�
}

// �J�����̏���
void Camera::Process() {

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_LEFT)
	{

	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_RIGHT)
	{

	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_UP)
	{
		
	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetNowInput() & PAD_INPUT_DOWN)
	{

	}

	// �J�����̈ʒu�ƌ�����ݒ�
	SetCameraPositionAndTarget_UpVecY(position, VGet(5, 0, 5));
}

// �R���X�g���N�^
Camera::Camera() {

	// ����������
	Initialize();
}

// �f�X�g���N�^
Camera::~Camera() {

}