#pragma once

#include "DxLib.h"
#include "Stage.h"

constexpr float CAMERA_ANGLE_SPEED = 2.0f;           // �J�����̎��_�ړ����x
constexpr float CAMERA_PLAYER_HEIGHT = 5.0f;         // �v���C���[�̍��W����ǂꂾ�������ʒu�ɂ��邩
constexpr float CAMERA_STAGE_DISTANCE = STAGE_WIDTH; // �J�����ƃX�e�[�W�̋���
constexpr VECTOR POSITION_0_90_180_270[4] =          // �J�����̎��_���Ƃ̍��W
{
	{ (CAMERA_STAGE_DISTANCE * 2) - 1, 5.0f, STAGE_WIDTH_CENTER },
	{ STAGE_WIDTH_CENTER, 5.0f, -CAMERA_STAGE_DISTANCE },
	{ -CAMERA_STAGE_DISTANCE, 5.0f, STAGE_WIDTH_CENTER },
	{STAGE_WIDTH_CENTER, 5.0f, (CAMERA_STAGE_DISTANCE * 2) - 1 }
};

class Camera
{
private:
	VECTOR position; // �ʒu
	VECTOR target;   // �����_���W
	int posNum;      // ���W�ԍ�

public:
	void Initialize(); // ����������
	void Process();    // �J�����̏���
	VECTOR GetPos() { return position; }  // �J�����̍��W���擾
	VECTOR GetTarget() { return target; } // �J�����̃^�[�Q�b�g���W���擾

	Camera();
	~Camera();
};

extern Camera camera; // �J����