#pragma once

#include "Block.h"

constexpr int STAGE_WIDTH = 10;                                                   // �X�e�[�W�̕�(x, z)
constexpr int STAGE_HEIGHT = 120;                                                 // �X�e�[�W�̍���
constexpr int STAGE_DIFFICULTY_SPACE = 30;                                        // ��Փx���ς��Ԋu(y���W)
constexpr float STAGE_WIDTH_CENTER = ((static_cast<float>(STAGE_WIDTH) - 1) / 2); // �X�e�[�W�̕��̒���

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

class Stage {
private:
	static Block blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH]; // �u���b�N�̔z�u
	int difficulty[STAGE_HEIGHT / STAGE_DIFFICULTY_SPACE];               // �X�e�[�W�̓�Փx
	void (*func)(VECTOR);                                                // �߂�l�̂Ȃ��֐��|�C���^�̒�`
	static void RenderFunc(VECTOR pos);                                  // �X�e�[�W�̕`��(�֐��|�C���^�Ŏw��)
	static void SetDataInBlockFunc(VECTOR pos);                          // �u���b�N�Ƀf�[�^���Z�b�g(�֐��|�C���^�Ŏw��)
public:
	void Create();                                                       // �X�e�[�W�̐���
	void Render();                                                       // �X�e�[�W�̕`��
	void SetDataInBlock();                                               // �u���b�N�Ƀf�[�^���Z�b�g
	void LoadBlockData();                                                // �u���b�N�̃f�[�^��ǂݍ���
	void ProcessEverything(void (*func)(VECTOR pos));                    // �S�Ẵu���b�N�ɑ΂��ď������s��

	Stage(int startDifficulty);

	int GetBlockPlacement(VECTOR pos);
	void InitBlockPlacement(VECTOR pos);
	void SetBlockPlacement(VECTOR pos, int num);
};