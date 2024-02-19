#include <stdio.h>
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
	FuncProcess(rand() % 1);
}

// �֐��|�C���^�ɃZ�b�g
void CREATE_PROCESS::SetFunc(void (*newFunc)(), int num) {
	func[num] = newFunc;
}

// �X�e�[�W�����������s�������̔ԍ����擾(reverse true : �����𔽓]�������l false : ���̂܂܂̐��l)
int CREATE_PROCESS::GetRandDir(bool reverse) {

	// �����𔽓]�������l���擾����Ȃ�
	// �����𔽓]�������l�ɉ��H���ĕԂ�
	if (reverse) {
		randDir += 2;
		if (randDir >= 4) {
			randDir -= 4;
		}
	}

	return randDir;
}

// �X�e�[�W�����������s���ʒu�̐��l��͈͓��Ɏ��߂�
void CREATE_PROCESS::ClampCreationPos() {
	if (creationPos.x < 0)
		creationPos.x = 0;
	if (creationPos.y < 0)
		creationPos.y = 0;
	if (creationPos.z < 0)
		creationPos.z = 0;

	if (creationPos.x > STAGE_WIDTH - 1)
		creationPos.x = STAGE_WIDTH - 1;
	if (creationPos.y > STAGE_HEIGHT - 1)
		creationPos.y = STAGE_HEIGHT - 1;
	if (creationPos.z > STAGE_WIDTH - 1)
		creationPos.z = STAGE_WIDTH - 1;
}

// �����������_���Ŏw��(4����)
void CREATE_PROCESS::SetRandDir() {

	while (true)
	{
		// 0�`4�̗����𐶐�
		randDir = rand() % 4;

		// �O��̌����̔��΂̌������͑O��Ɠ��������Ȃ��蒼��
		int reverseOldDir = oldDir += 2;
		if (reverseOldDir > 3) {
			reverseOldDir -= 4;
		}
		if ((randDir == reverseOldDir) || (randDir == oldDir)) {
			continue;
		}

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

		// �w�肵�������̃u���b�N���X�e�[�W�͈̔͊O�Ȃ��蒼��
		if (!stage.CheckPos(VAdd(creationPos, creationDir)))
			continue;

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

	// �X�e�[�W�����̈ʒu�ƌ������擾
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();

	// �X�e�[�W�J�n���̏��𐶐�
	StartFloor();

	// ���̃X�e�[�W�Ŏg�p����X�e�[�W���������̊֐��|�C���^�z��ɁA�X�e�[�W���������̊֐��|�C���^���Z�b�g
	SetStageFunc();

#if 0
	// �ŏ��̊K�i��ݒu
	++pos->y;
	createProcess.ClampCreationPos();
	stage.SetBlock(*pos, -2, createProcess.GetRandDir(true));
	*pos = VAdd(*pos, *dir);
	createProcess.ClampCreationPos();
#endif // 0
}

// �X�e�[�W�J�n���̏��𐶐�
void TEST_CREATE_PROCESS::StartFloor() {

	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_WIDTH; ++j) {
			pos.x = static_cast<float>(i);
			pos.z = static_cast<float>(j);
			stage.SetBlock(pos, 2, -1);
		}
	}
}

// ��{���𐶐�
void TEST_CREATE_PROCESS::OneLoad() {

	// �X�e�[�W�����̌����������_���ɕύX
	createProcess.SetRandDir();

	int createCount = 0;

	// �X�e�[�W�̒[�܂ł̋����𐔂���
	while (true) {
		if (stage.CheckPos(VAdd(*pos, VScale(*dir, static_cast<float>(createCount))))) {
			++createCount;
		}
		else
			break;
	}

	// ��������u���b�N�̒���������
	if (createCount > 7)
		createCount = 7;
	createCount = rand() % createCount + 1;

	// ��i�オ�邩�����Ō���A�������O��Ă��ݒu�ʒu�̉��Ƀu���b�N������Έ�i�オ��
	if ((rand() % 16 < 4) || (!stage.CheckBlock(VAdd(*pos, VGet(0.0f, -1.0f, 0.0f))))) {

		// ��i�グ�鏈��
		++pos->y;
		createProcess.ClampCreationPos();
		stage.SetBlock(*pos, -2, createProcess.GetRandDir(true));
		*pos = VAdd(*pos, *dir);
		createProcess.ClampCreationPos();
		--createCount;
	}

	// �X�e�[�W����
	stage.SetBlock(*pos, VAdd(*pos, VScale(*dir, static_cast<float>(createCount))), 1, -1);
	*pos = VAdd(*pos, VScale(*dir, static_cast<float>(createCount)));
	createProcess.ClampCreationPos();
	createCount = 0;
}

// ����2�}�X�̈�{���𐶐�
void TEST_CREATE_PROCESS::OnrLoad_2Squares() {

}

// �����`�̏��𐶐�
void TEST_CREATE_PROCESS::Floor() {

}