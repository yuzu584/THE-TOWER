#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;

// �֐��|�C���^�ɃZ�b�g
void CREATE_PROCESS::SetFunc(void (*newFunc)(VECTOR*), int num) {
	func[num] = newFunc;
}

// ���̃X�e�[�W�Ŏg�p����X�e�[�W���������̊֐��|�C���^�z��ɁA�X�e�[�W���������̊֐��|�C���^���Z�b�g
void TEST_CREATE_PROCESS::SetStageFunc() {
	createProcess.SetFunc(StartFloor, 0);
	createProcess.SetFunc(OneLoad, 1);
	createProcess.SetFunc(OnrLoad_2Squares, 2);
	createProcess.SetFunc(StartFloor, 3);
	createProcess.SetFunc(StartFloor, 4);
}

// �X�e�[�W������
void TEST_CREATE_PROCESS::Initialize() {
	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_WIDTH; ++j) {
			pos.x = static_cast<float>(i);
			pos.z = static_cast<float>(j);
			stage.SetBlockPlacement(pos, 2);
		}
	}
}

// �X�e�[�W�J�n���̏��𐶐�
void TEST_CREATE_PROCESS::StartFloor(VECTOR* pos) {

}

// ��{���𐶐�
void TEST_CREATE_PROCESS::OneLoad(VECTOR* pos) {
	stage.SetBlockPlacement(*pos, 1);
}

// ����2�}�X�̈�{���𐶐�
void TEST_CREATE_PROCESS::OnrLoad_2Squares(VECTOR* pos) {

}