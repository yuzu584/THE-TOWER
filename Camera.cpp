#include "Camera.h"
#include "Input.h"
#include "Player.h"

#include <math.h>

CAMERA camera; // �J������錾

// ����������
void CAMERA::Initialize() {
	posNum = 1;
	target = VGet(STAGE_WIDTH_CENTER, 0.0f, STAGE_WIDTH_CENTER); // �����_��ݒ�
}

// �J�����̏���
void CAMERA::Process() {

	// �u���v�{�^����������Ă������̏���
	if (input.GetEdgeInput() & PAD_INPUT_LEFT)
	{
		--posNum;
		if (posNum < 0)
			posNum = 3;
	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetEdgeInput() & PAD_INPUT_RIGHT)
	{
		++posNum;
		if (posNum > 3)
			posNum = 0;
	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetEdgeInput() & PAD_INPUT_UP)
	{
		
	}

	// �u���v�{�^����������Ă������̏���
	if (input.GetEdgeInput() & PAD_INPUT_DOWN)
	{
		
	}

	// �J�����̈ʒu�ƌ�����ݒ�
	position = POSITION_0_90_180_270[posNum];

	if ((posNum == 1) || (posNum == 3))
		position.x = player.GetPosition().x;
	if ((posNum == 0) || (posNum == 2))
		position.z = player.GetPosition().z;

	position.y = player.GetPosition().y + CAMERA_PLAYER_HEIGHT;

	target = player.GetPosition();

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