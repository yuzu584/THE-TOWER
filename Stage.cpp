#include "DxLib.h"
#include "Stage.h"

#include <fstream>

Stage stage;

// �ǂݍ��񂾃u���b�N�̃f�[�^�̍\����
LoadedBlockData loadedBlockData;

Block Stage::blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH];

// �X�e�[�W�̐���
void Stage::Create() {

}

// �X�e�[�W�̕`��
void Stage::Render() {
	func = &RenderFunc;
	ProcessEverything(func);
}

// �u���b�N�Ƀf�[�^���Z�b�g
void Stage::SetDataInBlock() {
	func = &SetDataInBlockFunc;
	ProcessEverything(func);
}

// �X�e�[�W�̕`��(�֐��|�C���^�Ŏw��)
void Stage::RenderFunc(VECTOR pos) {
	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].Render(pos);
}

// �u���b�N�Ƀf�[�^���Z�b�g(�֐��|�C���^�Ŏw��)
void Stage::SetDataInBlockFunc(VECTOR pos) {
	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(1);
}

// �u���b�N�̃f�[�^��ǂݍ���
void Stage::LoadBlockData() {
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

// �S�Ẵu���b�N�ɑ΂��ď������s��
void Stage::ProcessEverything(void (*func)(VECTOR pos)) {
	VECTOR pos;
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_HEIGHT; ++j) {
			for (int k = 0; k < STAGE_WIDTH; ++k) {
				pos.x = static_cast<float>(k);
				pos.y = static_cast<float>(j);
				pos.z = static_cast<float>(i);

				// �����œn���ꂽ����
				(func)(pos);
			}
		}
	}
}

// �X�e�[�W�̏�����
void Stage::Initialize(int startDifficulty) {
	LoadBlockData();                        // �u���b�N�̃f�[�^��ǂݍ���
	SetDataInBlock();                       // �u���b�N�̃f�[�^��������
	difficulty[0] = startDifficulty;        // �X�e�[�W�J�n���̓�Փx��ݒ�
}

// �w�肵�����W���X�e�[�W�͈͓̔�������
bool Stage::CheckPos(VECTOR pos) {
	return ((pos.x > 0) && (pos.y > 0) && (pos.z > 0) && (pos.x < STAGE_WIDTH) && (pos.y < STAGE_HEIGHT) && (pos.z < STAGE_WIDTH));
}

// �R���X�g���N�^
Stage::Stage() {

}

// �w�肵���ӏ��̃u���b�N���擾
Block Stage::GetBlockPlacement(VECTOR pos) {
	return blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)];
}

// �w�肵���ӏ��̃u���b�N�̎�ނ�������(�󔒃u���b�N�ɂ���)
void Stage::InitBlockPlacement(VECTOR pos) {

	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(0);
}

// �w�肵���ӏ��̃u���b�N�̃f�[�^��ݒ�
void Stage::SetBlockPlacement(VECTOR pos, int num) {

	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(num);
}