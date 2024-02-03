#include "Player.h"
#include "Camera.h"

// ����������
void PLAYER::Initialize() {

	// ���W��ݒ�
	position = VGet(0.0f, 1.0f, 0.0f);

	// ���f���ǂݍ���
	modelHandle = MV1LoadModel("BlockModel/WoodBox.mv1");
}

// �v���C���[�̏���
void PLAYER::Process() {
	VECTOR UpMoveVec;   // ����͎��̈ړ�����
	VECTOR LeftMoveVec; // �����͎��̈ړ�����
	VECTOR moveVec;     // �ŏI�I�Ȉړ�����
	int moveFlag;       // �ړ��������ǂ���( true : �ړ�����  false : �ړ����Ă��Ȃ� )

	// �ړ������x�N�g�����Z�o
	// ������̈ړ��x�N�g���̓J�����̎�����������Y�����𔲂�������
	UpMoveVec = VSub(camera.GetTarget(), camera.GetPos());
	UpMoveVec.y = 0.0f;

	// �������̈ړ��x�N�g���͏�����x�N�g����Y���̃v���X�����̃x�N�g���ɐ����ȕ���
	LeftMoveVec = VCross(UpMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// ��̃x�N�g���𐳋K��
	UpMoveVec = VNorm(UpMoveVec);
	LeftMoveVec = VNorm(LeftMoveVec);
}