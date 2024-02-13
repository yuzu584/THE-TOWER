#pragma once

#include "Stage.h"

constexpr int PROCESS_TYPE_AMOUNT = 5; // �X�e�[�W���Ƃ̐����v���Z�X�̐�

class CREATE_PROCESS {
private:
	void (*func[PROCESS_TYPE_AMOUNT])(VECTOR*);  // ���s����X�e�[�W���������̊֐��|�C���^�z��
	VECTOR creationPos = { 0.0f, 0.0f, 0.0f };        // �X�e�[�W�����������s���ʒu
public:
	void FuncProcess(int num) { (*func[num])(&creationPos); }       // �w�肵���֐��|�C���^�z��̊֐������s
	void SetFunc(void (*newfunc)(VECTOR*), int); // �֐��|�C���^���Z�b�g
	VECTOR* GetCreationPos() { return &creationPos; } // �X�e�[�W�����������s���ʒu���擾
};

// �e�X�g�X�e�[�W�����v���Z�X�̃N���X
class TEST_CREATE_PROCESS {
public:
	void SetStageFunc();									  // ���̃X�e�[�W�Ŏg�p����X�e�[�W���������̊֐��|�C���^�z��ɁA�X�e�[�W���������̊֐��|�C���^���Z�b�g
	void Initialize();									      // �X�e�[�W������
	static void StartFloor(VECTOR *pos);       // �X�e�[�W�J�n���̏��𐶐�
	static void OneLoad(VECTOR *pos);          // ��{���𐶐�
	static void OnrLoad_2Squares(VECTOR *pos); // ����2�}�X�̈�{���𐶐�
};

extern CREATE_PROCESS createProcess;
extern TEST_CREATE_PROCESS test_create_process;