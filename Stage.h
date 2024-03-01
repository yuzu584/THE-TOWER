#pragma once

#include "Block.h"

constexpr int STAGE_WIDTH = 10;                                                   // �X�e�[�W�̕�(x, z)
constexpr int STAGE_HEIGHT = 120;                                                 // �X�e�[�W�̍���
constexpr int STAGE_DIFFICULTY_SPACE = 30;                                        // ��Փx���ς��Ԋu(y���W)
constexpr float STAGE_WIDTH_CENTER = ((static_cast<float>(STAGE_WIDTH) - 1) / 2); // �X�e�[�W�̕��̒���
constexpr float STAGE_CREATE_HEIGHT_TO_PLAYER = 2.0f;                             // �X�e�[�W�����̍ۂɃv���C���[�̈ʒu���牽�u���b�N�����ʒu�𐶐��ʒu�ɂ��邩

// �ǂݍ��񂾃u���b�N�̃f�[�^�̍\����
struct LoadedBlockData
{
	// �ǂݍ��񂾃u���b�NID�̔z��
	int loadedId[BLOCK_TYPE_AMOUNT];

	// �ǂݍ��񂾃u���b�N���̔z��
	std::string loadedName[BLOCK_TYPE_AMOUNT];

	// �ǂݍ��񂾃��f���̃p�X�̔z��
	std::string loadedModelPath[BLOCK_TYPE_AMOUNT];

	// �ǂݍ��񂾃��f���n���h���̔z��
	int loadedModelHandle[BLOCK_TYPE_AMOUNT];
};

extern LoadedBlockData loadedBlockData;

class STAGE {
private:
	static BLOCK blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH];             // �u���b�N�̔z�u
	int difficulty[STAGE_HEIGHT / STAGE_DIFFICULTY_SPACE];                           // �X�e�[�W�̓�Փx
	void (*func)(VECTOR, int, int);                                                  // �߂�l�̂Ȃ��֐��|�C���^�̒�`
	static void RenderFunc(VECTOR pos, int num, int dir);                            // �X�e�[�W�̕`��(�֐��|�C���^�Ŏw��)
	static void SetBlockFunc(VECTOR pos, int num, int dir);                          // �u���b�N�Ƀf�[�^���Z�b�g(�֐��|�C���^�Ŏw��)
public:
	void Process();                                                                  // �X�e�[�W�̏���
	void Create();                                                                   // �X�e�[�W�̐���
	void Render();                                                                   // �X�e�[�W�̕`��
	void SetBlock(int num, int dir);                                                 // �u���b�N���Z�b�g(�S��)
	void SetBlock(VECTOR pos, int num, int dir);                                     // �u���b�N���Z�b�g
	void SetBlock(VECTOR pos1, VECTOR pos2, int num, int dir);                       // �u���b�N���Z�b�g(�͈�)
	void LoadBlockData();                                                            // �u���b�N�̃f�[�^��ǂݍ���
	void ProcessEverything(void (*func)(VECTOR, int, int), int num, int dir);                      // �S�Ẵu���b�N�ɑ΂��Ĉꊇ�������s��
	void ProcessRange(void (*func)(VECTOR, int, int), VECTOR pos1, VECTOR pos2, int num, int dir); // �w�肵���͈͂̃u���b�N�ɑ΂��Ĉꊇ�������s��
	void Initialize(int startDifficulty);											 // �X�e�[�W�̏�����
	bool CheckPos(VECTOR pos);                                                       // �w�肵�����W���X�e�[�W�͈͓̔�������
	bool CheckPos(VECTOR pos1, VECTOR pos2);                                         // �w�肵�����W���X�e�[�W�͈͓̔�������(�͈�)
	bool CheckBlock(VECTOR pos);                                                     // �w�肵�����W�Ƀu���b�N�����݂��邩����
	bool CheckBlock(VECTOR pos1, VECTOR pos2);                                       // �w�肵�����W�Ƀu���b�N�����݂��邩����(�͈�)

	STAGE();

	BLOCK GetBlockPlacement(VECTOR pos); // �w�肵���ӏ��̃u���b�N���擾
	void InitBlockPlacement(VECTOR pos);
};

extern STAGE stage;