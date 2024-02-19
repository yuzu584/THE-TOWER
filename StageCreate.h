#pragma once

#include "Stage.h"

constexpr int PROCESS_TYPE_AMOUNT = 5; // �X�e�[�W���Ƃ̐����v���Z�X�̐�

class CREATE_PROCESS {
private:
	void (*func[PROCESS_TYPE_AMOUNT])();                      // ���s����X�e�[�W���������̊֐��|�C���^�z��
	VECTOR creationPos = { 3.0f, 0.0f, 3.0f };                // �X�e�[�W�����������s���ʒu
	VECTOR creationDir = { 1.0f, 0.0f, 0.0f };                // �X�e�[�W�����������s������
	int randDir;                                              // �X�e�[�W�����������s�������̔ԍ�
	int oldDir = 1;                                           // �O��̃X�e�[�W�����������s�������̔ԍ�
public:
	void Process();                                           // �X�e�[�W��������
	void FuncProcess(int num) { (*func[num])(); }             // �w�肵���֐��|�C���^�z��̊֐������s
	void SetFunc(void (*newfunc)(), int);		              // �֐��|�C���^���Z�b�g
	void ClampCreationPos();                                  // �X�e�[�W�����������s���ʒu�̐��l��͈͓��Ɏ��߂�
	void SetRandDir();                                        // �����������_���Ŏw��(4����)
	void SetOldDir() { oldDir = randDir; }                    // �O��̃X�e�[�W�����������s�������̔ԍ����Z�b�g
	int GetRandDir(bool reverse);                             // �X�e�[�W�����������s�������̔ԍ����擾(reverse true : �����𔽓]�������l false : ���̂܂܂̐��l)
	VECTOR* GetCreationPos() { return &creationPos; }         // �X�e�[�W�����������s���ʒu���擾
	VECTOR* GetCreationDir() { return &creationDir; }         // �X�e�[�W�����������s���������擾
};

// �e�X�g�X�e�[�W�����v���Z�X�̃N���X
class TEST_CREATE_PROCESS {
private:
	static VECTOR* pos;                                       // �X�e�[�W�����������s���ʒu��������ϐ�
	static VECTOR* dir;                                       // �X�e�[�W�����������s��������������ϐ�
public:
	void SetStageFunc();									  // �X�e�[�W���������̊֐��|�C���^���Z�b�g
	void Initialize();									      // �X�e�[�W������
	void StartFloor();                                        // �X�e�[�W�J�n���̏��𐶐�
	static void OneLoad();                                    // ��{���𐶐�
	static void OnrLoad_2Squares();                           // ����2�}�X�̈�{���𐶐�
	static void Floor();                                      // �����`�̏��𐶐�
};

extern CREATE_PROCESS createProcess;
extern TEST_CREATE_PROCESS test_create_process;