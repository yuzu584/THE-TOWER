#include "Stage.h"
#include "DrawBoard.h"

#include <string>

// �u���b�N�̃f�[�^��������
void Block::SetData(int num) {

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

// �u���b�N�̃��f����`��
void Block::Render(VECTOR pos) {
	if (modelHandle != -1) {
		MV1SetScale(modelHandle, BLOCK_SCALE);
		MV1SetPosition(modelHandle, pos);
		MV1DrawModel(modelHandle);
	}
}

// �u���b�N��������
void Block::Initialize() {

	// ��C�u���b�N�̃f�[�^����
	SetData(0);

	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo(modelHandle, -1);
}

// �R���X�g���N�^
Block::Block() {

	// �u���b�N��������
	Initialize();
}

// �u���b�NID���擾
int Block::GetId() {
	return id;
}

// �u���b�N�����擾
std::string Block::GetName() {
	return name;
}

// �u���b�N�̌������擾
int Block::GetDirection() {
	return direction;
}

// �u���b�N�̌������w��
void Block::SetDirection(int num) {
	direction = num;
}