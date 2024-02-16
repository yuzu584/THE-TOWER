#include "Stage.h"
#include "DrawBoard.h"

#include <string>

// �u���b�N�̃f�[�^��������
void BLOCK::SetData(int num) {

	// ���łɃ��f���������ς݂Ȃ�
	if (modelHandle != -1) {

		// ���f�����폜
		MV1DeleteModel(modelHandle);

		// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
		MV1SetupCollInfo(modelHandle, -1);
	}

	// �u���b�NID���玩���̃u���b�N�̃f�[�^��T��
	int i = 0;
	while (num != loadedBlockData.loadedId[i])
	{
		if (i >= BLOCK_TYPE_AMOUNT - 1) { break; }
		++i;
	}

	// �T�����u���b�NID�̃f�[�^����
	id = loadedBlockData.loadedId[i];
	name = loadedBlockData.loadedName[i];
	modelHandle = MV1DuplicateModel(loadedBlockData.loadedModelHandle[i]);
}

// �u���b�N�̃f�[�^�ƌ�����������
void BLOCK::SetData(int num, int dir) {

	// ���łɃ��f���������ς݂Ȃ�
	if (modelHandle != -1) {

		// ���f�����폜
		MV1DeleteModel(modelHandle);

		// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
		MV1SetupCollInfo(modelHandle, -1);
	}

	// �u���b�NID���玩���̃u���b�N�̃f�[�^��T��
	int i = 0;
	while (num != loadedBlockData.loadedId[i])
	{
		if (i >= BLOCK_TYPE_AMOUNT - 1) { break; }
		++i;
	}

	// �T�����u���b�NID�̃f�[�^����
	id = loadedBlockData.loadedId[i];
	name = loadedBlockData.loadedName[i];
	modelHandle = MV1DuplicateModel(loadedBlockData.loadedModelHandle[i]);

	// ������ݒ�
	if(dir != -1)
		SetDirection(dir);
}

// �u���b�N�̃��f����`��
void BLOCK::Render(VECTOR pos) {
	if (modelHandle != -1) {
		MV1SetScale(modelHandle, BLOCK_SCALE);
		MV1SetPosition(modelHandle, pos);
		MV1DrawModel(modelHandle);
	}
}

// �u���b�N��������
void BLOCK::Initialize() {

	// ��C�u���b�N�̃f�[�^����
	SetData(0);

	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo(modelHandle, -1);
}

// �R���X�g���N�^
BLOCK::BLOCK() {

	// �u���b�N��������
	Initialize();
}

// �u���b�NID���擾
int BLOCK::GetId() {
	return id;
}

// �u���b�N�����擾
std::string BLOCK::GetName() {
	return name;
}

// �u���b�N�̌������擾
int BLOCK::GetDirection() {
	return direction;
}

// �u���b�N�̌������w��
void BLOCK::SetDirection(int num) {
	direction = num;
	MV1SetRotationXYZ(modelHandle, BLOCK_DIRECTION[num]);
}