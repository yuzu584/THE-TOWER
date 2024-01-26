#pragma once

#include "DxLib.h"

constexpr float CAMERA_ANGLE_SPEED = 2.0f; // �J�����̎��_�ړ����x
constexpr VECTOR POSITION_0_90_180_270[4] = // �J�����̎��_���Ƃ̍��W
{
	{ 15.0f, 5.0f, 5.0f },
	{ 5.0f, 5.0f, -10.0f },
	{ -10.0f, 5.0f, 5.0f },
	{ 5.0f, 5.0f, -15.0f }
};

class Camera
{
private:
	VECTOR position; // �ʒu
	VECTOR target;   // �����_���W
	int posNum;      // ���W�ԍ�

public:
	void Initialize();           // ����������
	void Process();              // �J�����̏���

	Camera();
	~Camera();
};

extern Camera camera; // �J����