#include "Stage.h"
#include "StageCreate.h"
#include "Player.h"

#include <fstream>

STAGE stage;

// �ǂݍ��񂾃u���b�N�̃f�[�^�̍\����
LoadedBlockData loadedBlockData;

BLOCK STAGE::blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH];

// �X�e�[�W�̏���
void STAGE::Process() {
	
	// �X�e�[�W�𐶐�
	while (player.GetPosition().y + 5.0f > createProcess.GetCreationPos()->y)
	{
		Create();
	}

	// �X�e�[�W�̕`��
	Render();
}

// �X�e�[�W�̐���
void STAGE::Create() {
	createProcess.Process();
}

// �X�e�[�W�̕`��
void STAGE::Render() {
	func = &RenderFunc;
	ProcessEverything(func, 0, 1);
}

// �u���b�N�Ƀf�[�^���Z�b�g(�S��)
void STAGE::SetBlock(int num, int dir) {
	func = &SetBlockFunc;
	ProcessEverything(func, num, dir);
}

// �u���b�N�Ƀf�[�^���Z�b�g
void STAGE::SetBlock(VECTOR pos, int num, int dir) {

	// �w�肵�����W���X�e�[�W�͈͓̔��Ȃ�u���b�N��z�u
	if (CheckPos(pos)) {
		createProcess.SetOldDir();
		blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(num, dir);
	}
}

// �u���b�N�Ƀf�[�^���Z�b�g(�͈�)
void STAGE::SetBlock(VECTOR pos1, VECTOR pos2, int num, int dir) {

	// �w�肵�����W���X�e�[�W�͈͓̔��Ȃ�u���b�N��z�u
	func = &SetBlockFunc;
	ProcessRange(func, pos1, pos2, num, dir);
}

// �X�e�[�W�̕`��(�֐��|�C���^�Ŏw��)
void STAGE::RenderFunc(VECTOR pos, int num, int dir) {
	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].Render(pos);
}

// �u���b�N�Ƀf�[�^���Z�b�g(�֐��|�C���^�Ŏw��)
void STAGE::SetBlockFunc(VECTOR pos, int num, int dir) {

	// �w�肵�����W���X�e�[�W�͈͓̔��Ȃ�u���b�N��z�u
	if (stage.CheckPos(pos)) {
		createProcess.SetOldDir();
		blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(num, dir);
	}
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
void STAGE::ProcessEverything(void (*func)(VECTOR pos, int num, int dir), int num, int dir) {
	VECTOR pos;
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_HEIGHT; ++j) {
			for (int k = 0; k < STAGE_WIDTH; ++k) {
				pos.x = static_cast<float>(i);
				pos.y = static_cast<float>(j);
				pos.z = static_cast<float>(k);

				// �����œn���ꂽ����
				(func)(pos, num, dir);
			}
		}
	}
}

// �w�肵���͈͂̃u���b�N�ɑ΂��Ĉꊇ�������s��
void STAGE::ProcessRange(void (*func)(VECTOR pos, int num, int dir), VECTOR pos1, VECTOR pos2, int num, int dir){
	VECTOR Pos;
	VECTOR Pos1;
	VECTOR Pos2;

	// �n���ꂽ2��VECTOR�̈����ǂ��炪�傫�����ŕ���
	if (VSize(pos1) <= VSize(pos2)) {
		Pos1 = pos1;
		Pos2 = pos2;
	}
	else {
		Pos1 = pos2;
		Pos2 = pos1;
	}

	for (int i = static_cast<int>(round(Pos1.x)); i <= static_cast<int>(round(Pos2.x)); ++i) {
		for (int j = static_cast<int>(round(Pos1.y)); j <= static_cast<int>(round(Pos2.y)); ++j) {
			for (int k = static_cast<int>(round(Pos1.z)); k <= static_cast<int>(round(Pos2.z)); ++k) {
				Pos.x = static_cast<float>(i);
				Pos.y = static_cast<float>(j);
				Pos.z = static_cast<float>(k);

				// �����œn���ꂽ����
				if(CheckPos(Pos))
					(func)(Pos, num, dir);
			}
		}
	}
}

// �X�e�[�W�̏�����
void STAGE::Initialize(int startDifficulty) {
	LoadBlockData();                        // �u���b�N�̃f�[�^��ǂݍ���
	SetBlock(0, 1);                         // �u���b�N�̃f�[�^��������
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

// �w�肵�����W���X�e�[�W�͈͓̔�������(�͈�)
bool STAGE::CheckPos(VECTOR pos1, VECTOR pos2) {
	VECTOR Pos;
	VECTOR Pos1;
	VECTOR Pos2;

	// �n���ꂽ2��VECTOR�̈����ǂ��炪�傫�����ŕ���
	if (VSize(pos1) <= VSize(pos2)) {
		Pos1 = pos1;
		Pos2 = pos2;
	}
	else {
		Pos1 = pos2;
		Pos2 = pos1;
	}

	for (int i = static_cast<int>(round(Pos1.x)); i <= static_cast<int>(round(Pos2.x)); ++i) {
		for (int j = static_cast<int>(round(Pos1.y)); j <= static_cast<int>(round(Pos2.y)); ++j) {
			for (int k = static_cast<int>(round(Pos1.z)); k <= static_cast<int>(round(Pos2.z)); ++k) {
				Pos.x = static_cast<float>(i);
				Pos.y = static_cast<float>(j);
				Pos.z = static_cast<float>(k);

				// �w�肵�����W���X�e�[�W�͈͓̔�������
				if (!CheckPos(Pos))
					return false;
			}
		}
	}

	return true;
}

// �w�肵�����W�Ƀu���b�N�����݂��邩����
bool STAGE::CheckBlock(VECTOR pos) {

	// �w��̍��W�̃u���b�N�̃��f���n���h�����擾���āA��C�u���b�N�ȊO�Ȃ瑶�݂��锻��ɂ���
	int id = 0;
	if(CheckPos(pos))
		id = GetBlockPlacement(pos).GetId();

	return id == 0; // 0 �͋�C�u���b�N��ID
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