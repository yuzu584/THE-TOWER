#include "Stage.h"
#include "StageCreate.h"

#include <fstream>

STAGE stage;

// �ǂݍ��񂾃u���b�N�̃f�[�^�̍\����
LoadedBlockData loadedBlockData;

BLOCK STAGE::blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH];

// �X�e�[�W�̐���
void STAGE::Create() {
	createProcess.Process();
}

// �X�e�[�W�̕`��
void STAGE::Render() {
	func = &RenderFunc;
	ProcessEverything(func, 0);
}

// �u���b�N�Ƀf�[�^���Z�b�g
void STAGE::SetBlock(int num) {
	func = &SetBlockFunc;
	ProcessEverything(func, num);
}

// �u���b�N�Ƀf�[�^���Z�b�g(�͈�)
void STAGE::SetBlock_Range(VECTOR pos1, VECTOR pos2, int num) {
	func = &SetBlockFunc;
	ProcessRange(func, pos1, pos2, num);
}

// �X�e�[�W�̕`��(�֐��|�C���^�Ŏw��)
void STAGE::RenderFunc(VECTOR pos, int num) {
	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].Render(pos);
}

// �u���b�N�Ƀf�[�^���Z�b�g(�֐��|�C���^�Ŏw��)
void STAGE::SetBlockFunc(VECTOR pos, int num) {
	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(num);
}

// �u���b�N�̃f�[�^��ǂݍ���
void STAGE::LoadBlockData() {
	std::string line;  // �ǂݍ��񂾕�����
	int lineCount = 0; // �s�����J�E���g

	// �ǂݍ��ރt�@�C���̃p�X���w��
	std::ifstream file("BlockData.txt");

	// 1�s���ǂݍ���Ŋi�[����
	while (std::getline(file, line)) {

		// �ǂݍ��񂾃f�[�^�����s�Ȃ珈�����s��Ȃ�
		if (line == "")
			continue;

		// �s���ŕ���
		switch (lineCount % 3)
		{
		case 0:
			loadedBlockData.loadedId[lineCount / 3] = stoi(line);
			break;
		case 1:
			loadedBlockData.loadedName[lineCount / 3] = line;
			break;
		case 2:
			loadedBlockData.loadedModelPath[lineCount / 3] = line;
			break;
		default:
			break;
		}
		++lineCount;
	}

	// ���f���̓ǂݍ���
	for (int i = 0; i < BLOCK_TYPE_AMOUNT; i++)
	{
		loadedBlockData.loadedModelHandle[i] = MV1LoadModel(loadedBlockData.loadedModelPath[i].c_str());
	}
}

// �S�Ẵu���b�N�ɑ΂��Ĉꊇ�������s��
void STAGE::ProcessEverything(void (*func)(VECTOR pos, int num), int num) {
	VECTOR pos;
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_HEIGHT; ++j) {
			for (int k = 0; k < STAGE_WIDTH; ++k) {
				pos.x = static_cast<float>(i);
				pos.y = static_cast<float>(j);
				pos.z = static_cast<float>(k);

				// �����œn���ꂽ����
				(func)(pos, num);
			}
		}
	}
}

// �w�肵���͈͂̃u���b�N�ɑ΂��Ĉꊇ�������s��
void STAGE::ProcessRange(void (*func)(VECTOR pos, int num), VECTOR pos1, VECTOR pos2, int num) {
	VECTOR pos;
	for (int i = static_cast<int>(round(pos1.x)); i <= static_cast<int>(round(pos2.x)); ++i) {
		for (int j = static_cast<int>(round(pos1.y)); j <= static_cast<int>(round(pos2.y)); ++j) {
			for (int k = static_cast<int>(round(pos1.z)); k <= static_cast<int>(round(pos2.z)); ++k) {
				pos.x = static_cast<float>(i);
				pos.y = static_cast<float>(j);
				pos.z = static_cast<float>(k);

				// �����œn���ꂽ����
				(func)(pos, num);
			}
		}
	}
}

// �X�e�[�W�̏�����
void STAGE::Initialize(int startDifficulty) {
	LoadBlockData();                        // �u���b�N�̃f�[�^��ǂݍ���
	SetBlock(0);                            // �u���b�N�̃f�[�^��������
	difficulty[0] = startDifficulty;        // �X�e�[�W�J�n���̓�Փx��ݒ�
}

// �w�肵�����W���X�e�[�W�͈͓̔�������
bool STAGE::CheckPos(VECTOR pos) {
	return ((static_cast<int>(round(pos.x)) > -1.0f) &&
		(static_cast<int>(round(pos.y)) > -1.0f) &&
		(static_cast<int>(round(pos.z)) > -1.0f) &&
		(pos.x < STAGE_WIDTH) &&
		(pos.y < STAGE_HEIGHT) &&
		(pos.z < STAGE_WIDTH));
}

// �R���X�g���N�^
STAGE::STAGE() {

}

// �w�肵���ӏ��̃u���b�N���擾
BLOCK STAGE::GetBlockPlacement(VECTOR pos) {
	return blockPlacement[static_cast<int>(round(pos.x))][static_cast<int>(round(pos.y))][static_cast<int>(round(pos.z))];
}

// �w�肵���ӏ��̃u���b�N�̎�ނ�������(�󔒃u���b�N�ɂ���)
void STAGE::InitBlockPlacement(VECTOR pos) {

	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(0);
}

// �w�肵���ӏ��̃u���b�N�̃f�[�^��ݒ�
void STAGE::SetBlockPlacement(VECTOR pos, int num) {

	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(num);
}