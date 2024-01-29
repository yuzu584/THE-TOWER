#pragma once
#include "DxLib.h"

constexpr float PLAYER_MOVE_SPEED        = 1.0f; // �ړ����x
constexpr float PLAYER_JUMP_POWER        = 1.0f; // �W�����v��
constexpr float PLAYER_GRAVITY           = 1.0f; // �d��
constexpr float PLAYER_ANGLE_SPEED       = 0.2f; // �p�x�ω����x
constexpr int   PLAYER_MAX_HITCOLL       = 1024; // ��������R���W�����|���S���̍ő吔
constexpr float PLAYER_ENUM_DEFAULT_SIZE = 3.0f; // ���͂̃|���S�����o�Ɏg�p���鋅�̏����T�C�Y
constexpr float PLAYER_HIT_WIDTH         = 0.5f; // �����蔻��J�v�Z���̔��a
constexpr float PLAYER_HIT_HEIGHT        = 0.5f; // �����蔻��J�v�Z���̍���
constexpr int   PLAYER_HIT_TRYNUM        = 16;   // �ǉ����o�������̍ő厎�s��
constexpr float PLAYER_HIT_SLIDE_LENGTH  = 0.1f; // ��x�̕ǉ����o�������ŃX���C�h�����鋗��

enum AnimeState {
	Neutral = 1,
	Run = 2,
	Jump = 3,
};

class Player {
private:
	VECTOR position; // ���W
	VECTOR targetDirection; // ���������̃x�N�g��
	float angle;            // �����Ă���p�x
	float speed_y;          // y���̑��x
	int modelHandle; // ���f���n���h��
	VECTOR moveVector_H; // �v���C���[�̍��E�̈ړ��x�N�g��
	VECTOR moveVector_V; // �v���C���[�̑O��̈ړ��x�N�g��
	VECTOR moveVector;   // �ŏI�I�Ȉړ��x�N�g��
	bool moveFlag; // �ړ��������ǂ���
	AnimeState state; // ���
public:
	void Initialize(); // �v���C���[�̏�����
	void Process(); // �v���C���[�̏���
	void Move(VECTOR moveVector); // �v���C���[�̈ړ�����
	void AngleProcess(); // �v���C���[�̌�����ς��鏈��
	void AnimProcess(); // �v���C���[�̃A�j���[�V��������
	Player();
	~Player();
};

extern Player player;