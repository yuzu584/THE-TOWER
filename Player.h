#pragma once
#include "DxLib.h"

constexpr float PLAYER_MOVE_SPEED = 0.05f; // �ړ����x
constexpr float PLAYER_JUMP_POWER = 0.2f; // �W�����v��
constexpr float PLAYER_GRAVITY = 0.01f; // �d��
constexpr float PLAYER_ANGLE_SPEED = 0.2f; // �p�x��ς��鑬�x
constexpr int  PLAYER_MAX_HITCOLL = 512; // ��������R���W�����|���S���̍ő吔
constexpr float PLAYER_COLL_SPHERE_SIZE = 1.0f;	// ���͂̃|���S�����o�Ɏg�p���鋅�̃T�C�Y
constexpr float PLAYER_HIT_WIDTH = 0.5f;		// �����蔻��J�v�Z���̔��a
constexpr float PLAYER_HIT_HEIGHT = 0.5f;		// �����蔻��J�v�Z���̍���
constexpr int PLAYER_HIT_TRYNUM = 16;		// �ǉ����o�������̍ő厎�s��
constexpr float PLAYER_HIT_SLIDE_LENGTH = 0.01f;		// ��x�̕ǉ����o�������ŃX���C�h�����鋗��

enum STATE
{
	neutral = 0,
	run = 1,
	jump = 2,
};

class PLAYER
{
public:
	void Initialize(); // ����������
	void Process();    // �v���C���[�̏���
	void AngleProcess(); // �����̏���
	void Move(VECTOR moveVec);         // �ړ�����

private:
	VECTOR position; // ���W
	int modelHandle; // ���f���n���h��
	STATE state;     // ����̏��
	float speed_y;   // Y���̈ړ����x
	float angle;     // ����
	VECTOR targetDirection; // �����Ώۂ̍��W
};

extern PLAYER player;