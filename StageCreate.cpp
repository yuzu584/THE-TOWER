#include <stdio.h>
#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;
int TEST_CREATE_PROCESS::createCount;

// �X�e�[�W��������
void CREATE_PROCESS::Process() {

	// �X�e�[�W����������
	test_create_process.Initialize();

	// �����_���ȃX�e�[�W�����������s��
	FuncProcess(rand() % 1);
}

// �֐��|�C���^�ɃZ�b�g
void CREATE_PROCESS::SetFunc(CREATE_PROCESS_FUNC newFunc, int num) {
	func[num] = newFunc;
}

// �X�e�[�W�����������s�������̔ԍ����擾(reverse true : �����𔽓]�������l false : ���̂܂܂̐��l)
int CREATE_PROCESS::GetRandDir(bool reverse) {

	// �����𔽓]�������l���擾����Ȃ�
	// �����𔽓]�������l�ɉ��H���ĕԂ�
	if (reverse) {
		randReverseDir = randDir;
		randReverseDir += 2;
		if (randReverseDir >= 4) {
			randReverseDir -= 4;
		}
		return randReverseDir;
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

		// �O��̌����̔��΂̌��������߂�
		int reverseOldDir = (oldDir + 2);
		if (reverseOldDir > 3) {
			reverseOldDir -= 4;
		}

		// �O��̌����̔��΂̌������͑O��Ɠ��������Ȃ��蒼��
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
	createProcess.SetFunc(OneLoad, 1);
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
			stage.SetBlock(pos, 2, -1);
		}
	}
}

// ��{���𐶐�
void TEST_CREATE_PROCESS::OneLoad() {
	
	// �X�e�[�W�����̈ʒu�ƌ������擾
	VECTOR& pos = createProcess.GetCreationPos();
	VECTOR& dir = createProcess.GetCreationDir();

	createCount = 0;
	MATRIX matrix;
	VECTOR vec1, vec2;
	int dirDiff = createProcess.GetRandDir(false); // �O��̃X�e�[�W���������Ƃ̍�

	// �x���𐶐�
	vec1 = dir;                             // �X�e�[�W�����̌�������
	matrix = MGetRotY(DX_PI_F / 2.0f);      // 90�x��]����s��
	vec2 = VTransform(vec1, matrix);        // vec1��90�x��]�������l����
	vec2 = VScale(vec2, ONELOAD_MAX_WIDTH); // ��{���̕��ŃX�P�[��
	stage.SetBlock(pos, VAdd(pos, vec2), 1, -1);

	// �X�e�[�W��������������ɍs���鏀���������܂ŌJ��Ԃ�
	while (true)
	{
		// �X�e�[�W�����̌����������_���ɕύX
		createProcess.SetRandDir();

		// �X�e�[�W�̒[�܂ł̋����𐔂���
		while (true) {
			if (stage.CheckPos(VAdd(pos, VScale(dir, static_cast<float>(createCount + 1))))) {
				++createCount;
			}
			else
				break;
		}

		// �[�܂ł̋�����0�u���b�N�Ȃ��蒼��
		if (createCount == 0) {
			createCount = 0;
			continue;
		}
		// �[�܂ł̋�����0�u���b�N�ȏ�Ȃ甲����
		else if (createCount > 0)
			break;
	}

	// ����ƑO��̃X�e�[�W���������̍������߂�
	dirDiff -= createProcess.GetRandDir(false);
	
	// �X�e�[�W�����̌����̍��ɂ���ĕ���
	switch (dirDiff)
	{
	case -1:
		pos = VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH));
		break;
	case -3:
		pos = VAdd(VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH + 1)), VGet(0.0f, 0.0f, static_cast<float>(ONELOAD_MAX_WIDTH - 1)));
		break;
	case 1:
		vec1 = dir;                             // �X�e�[�W�����̌�������
		matrix = MGetRotY(DX_PI_F / -2.0f);     // -90�x��]����s��
		vec2 = VTransform(vec1, matrix);        // vec1��-90�x��]�������l����
		vec2 = VScale(vec2, ONELOAD_MAX_WIDTH); // ��{���̕��ŃX�P�[��
		pos = VAdd(VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH)), vec2);
		break;
	case 3:
		pos = VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH));
		break;
	default:
		break;
	}
	createProcess.ClampCreationPos();

	// �O��̃u���b�N���������̌�����ݒ�
	createProcess.SetOldDir();

	// ��������u���b�N�̒���������
	createCount = rand() % createCount + 1;

	// ��{���ɕ����������鏈���Ŏg���ϐ��̒l��ݒ�
	vec1 = dir;                             // �X�e�[�W�����̌�������
	matrix = MGetRotY(DX_PI_F / 2.0f);      // 90�x��]����s��
	vec2 = VTransform(vec1, matrix);        // vec1��90�x��]�������l����
	vec2 = VScale(vec2, ONELOAD_MAX_WIDTH); // ��{���̕��ŃX�P�[��

	// ��i�オ�邩�����Ō���A�������O��Ă��ݒu�ʒu�̉��Ƀu���b�N������Έ�i�オ��
	if ((rand() % RAISE_UP_RATE) || (!stage.CheckBlock(VAdd(pos, VGet(0.0f, -1.0f, 0.0f)), VAdd(VAdd(pos, vec2), VGet(0.0f, -1.0f, 0.0f))))) {

		// ��i�グ�鏈��
		++pos.y;
		stage.SetBlock(pos, VAdd(pos, vec2), -2, createProcess.GetRandDir(true));
		pos = VAdd(pos, dir);
		createProcess.ClampCreationPos();
		--createCount;
	}

	// �X�e�[�W����
	for (int i = 0; i < createCount; ++i) {

		// �ݒu�ʒu�̉��Ƀu���b�N������Έ�i�オ��
		if (!stage.CheckBlock(VAdd(pos, VGet(0.0f, -1.0f, 0.0f)), VAdd(VAdd(pos, vec2), VGet(0.0f, -1.0f, 0.0f)))) {

			// ��i�グ�鏈��
			++pos.y;
			stage.SetBlock(pos, VAdd(pos, vec2), -2, createProcess.GetRandDir(true));
			pos = VAdd(pos, dir);
			createProcess.ClampCreationPos();
		}
		else {
			stage.SetBlock(pos, VAdd(pos, vec2), 1, -1);
			pos = VAdd(pos, dir);
			createProcess.ClampCreationPos();
		}
	}
	createCount = 0;
}

// ���̊J������{���𐶐�
void TEST_CREATE_PROCESS::HolesOneLoad() {

}

// �����`�̏��𐶐�
void TEST_CREATE_PROCESS::Floor() {

}

// ���̑��ꂩ��؂藣���ꂽ�Ɨ��������𐶐�
void TEST_CREATE_PROCESS::FloatingFloor() {

}