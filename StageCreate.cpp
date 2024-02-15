#include <stdio.h>
#include <time.h>
#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;
VECTOR* TEST_CREATE_PROCESS::pos;
VECTOR* TEST_CREATE_PROCESS::dir;

// �X�e�[�W��������
void CREATE_PROCESS::Process() {

	// �X�e�[�W����������
	test_create_process.Initialize();

	// ���ݎ��������Ƃɏ��������ė����𐶐����A
	// �����_���ȃX�e�[�W�����������s��
	FuncProcess(Random(1));
}

// �֐��|�C���^�ɃZ�b�g
void CREATE_PROCESS::SetFunc(void (*newFunc)(), int num) {
	func[num] = newFunc;
}

// �����𐶐�
int CREATE_PROCESS::Random(int num) {
	srand((unsigned int)time(NULL));
	return (rand() % num);
}

// �����𐶐�(�I�t�Z�b�g�Ő��l�����炷)
int CREATE_PROCESS::Random(int num, int offset) {
	srand((unsigned int)time(NULL));
	return ((rand() % num) + offset);
}

// �����������_���Ŏw��(4����)
void CREATE_PROCESS::SetRandDir() {

	// 0�`4�̗����𐶐�
	int randDir = Random(4);

	// �������������ɂ���ĕ���
	switch (randDir)
	{
	case 0:
		creationDir = { 1.0f, 0.0f, 0.0f };
		break;
	case 1:
		creationDir = { 0.0f, 0.0f, -1.0f };
		break;
	case 2:
		creationDir = { -1.0f, 0.0f, 0.0f };
		break;
	case 3:
		creationDir = { 0.0f, 0.0f, 1.0f };
		break;
	default:
		break;
	}
}

// ���̃X�e�[�W�Ŏg�p����X�e�[�W���������̊֐��|�C���^�z��ɁA�X�e�[�W���������̊֐��|�C���^���Z�b�g
void TEST_CREATE_PROCESS::SetStageFunc() {
	createProcess.SetFunc(OneLoad, 0);
	createProcess.SetFunc(OnrLoad_2Squares, 1);
	createProcess.SetFunc(Floor, 2);
	createProcess.SetFunc(Floor, 3);
	createProcess.SetFunc(Floor, 4);
}

// �X�e�[�W������
void TEST_CREATE_PROCESS::Initialize() {

	// �X�e�[�W�J�n���̏��𐶐�
	StartFloor();

	// ���̃X�e�[�W�Ŏg�p����X�e�[�W���������̊֐��|�C���^�z��ɁA�X�e�[�W���������̊֐��|�C���^���Z�b�g
	SetStageFunc();
}

// �X�e�[�W�J�n���̏��𐶐�
void TEST_CREATE_PROCESS::StartFloor() {

	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_WIDTH; ++j) {
			pos.x = static_cast<float>(i);
			pos.z = static_cast<float>(j);
			stage.SetBlock(pos, 2);
		}
	}
}

// ��{���𐶐�
void TEST_CREATE_PROCESS::OneLoad() {

	// �X�e�[�W�����̌����������_���ɕύX
	createProcess.SetRandDir();

	// �X�e�[�W�����̈ʒu�ƌ������擾
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();

	// ��i�オ�邩�����Ō���
	if (createProcess.Random(16) < 8) {
		pos->y += 1;
		stage.SetBlock(*pos, -2);
		*pos = VAdd(*pos, *dir);
	}

	// �X�e�[�W����
	for (int i = 0; i < 9; ++i) {
		stage.SetBlock(*pos, 1);
		*pos = VAdd(*pos, *dir);
	}
}

// ����2�}�X�̈�{���𐶐�
void TEST_CREATE_PROCESS::OnrLoad_2Squares() {
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();
	stage.SetBlock(*pos, VAdd(*pos, VGet(1.0f, 0.0f, 5.0f)), 1);
}

// �����`�̏��𐶐�
void TEST_CREATE_PROCESS::Floor() {
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();
	int width = createProcess.Random(5);
	stage.SetBlock(*pos, VAdd(*pos, VGet(static_cast<float>(width), 0.0f, static_cast<float>(width))), 1);
}