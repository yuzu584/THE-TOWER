#pragma once
#include "DxLib.h"

constexpr float PLAYER_MOVE_SPEED = 0.01f; // �ړ����x
constexpr float PLAYER_JUMP_POWER = 0.05f; // �W�����v��
constexpr float PLAYER_GRAVITY = 0.01f; // �d��

class PLAYER
{
public:
	void Initialize(); // ����������
	void Process(); // �v���C���[�̏���

private:
	VECTOR position; // ���W
	int modelHandle; // ���f���n���h��
};