#include <stdio.h>
#include <time.h>
#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;

// �X�e�[�W��������
void CREATE_PROCESS::Process() {

	// �X�e�[�W����������
	test_create_process.Initialize();

	// ���ݎ��������Ƃɏ��������ė����𐶐����A
	// �����_���ȃX�e�[�W�����������s��
	srand((unsigned int)time(NULL));
	FuncProcess(rand() % PROCESS_TYPE_AMOUNT);
}

// �֐��|�C���^�ɃZ�b�g
void CREATE_PROCESS::SetFunc(void (*newFunc)(), int num) {
	func[num] = newFunc;
}

// ���̃X�e�[�W�Ŏg�p����X�e�[�W���������̊֐��|�C���^�z��ɁA�X�e�[�W���������̊֐��|�C���^���Z�b�g
void TEST_CREATE_PROCESS::SetStageFunc() {
	createProcess.SetFunc(OneLoad, 0);
	createProcess.SetFunc(OnrLoad_2Squares, 1);
	createProcess.SetFunc(OneLoad, 2);
	createProcess.SetFunc(OnrLoad_2Squares, 3);
	createProcess.SetFunc(OneLoad, 4);
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
			stage.SetBlockPlacement(pos, 2);
		}
	}
}

// ��{���𐶐�
void TEST_CREATE_PROCESS::OneLoad() {
	VECTOR* pos = createProcess.GetCreationPos();
	stage.SetBlock_Range(VGet(0.0f, 0.0f, 0.0f), VGet(1.0f, 1.0f, 1.0f), 1);
}

// ����2�}�X�̈�{���𐶐�
void TEST_CREATE_PROCESS::OnrLoad_2Squares() {
	VECTOR* pos = createProcess.GetCreationPos();
	stage.SetBlock_Range(VGet(0.0f, 0.0f, 0.0f), VGet(3.0f, 1.0f, 3.0f), -1);
}