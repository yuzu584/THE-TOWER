#include "Stage.h"
#include "StageCreate.h"

#include <fstream>

STAGE stage;

// 読み込んだブロックのデータの構造体
LoadedBlockData loadedBlockData;

BLOCK STAGE::blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH];

// ステージの生成
void STAGE::Create() {
	createProcess.Process();
}

// ステージの描画
void STAGE::Render() {
	func = &RenderFunc;
	ProcessEverything(func, 0);
}

// ブロックにデータをセット
void STAGE::SetBlock(int num) {
	func = &SetBlockFunc;
	ProcessEverything(func, num);
}

// ブロックにデータをセット(範囲)
void STAGE::SetBlock_Range(VECTOR pos1, VECTOR pos2, int num) {
	func = &SetBlockFunc;
	ProcessRange(func, pos1, pos2, num);
}

// ステージの描画(関数ポインタで指定)
void STAGE::RenderFunc(VECTOR pos, int num) {
	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].Render(pos);
}

// ブロックにデータをセット(関数ポインタで指定)
void STAGE::SetBlockFunc(VECTOR pos, int num) {
	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(num);
}

// ブロックのデータを読み込む
void STAGE::LoadBlockData() {
	std::string line;  // 読み込んだ文字列
	int lineCount = 0; // 行数をカウント

	// 読み込むファイルのパスを指定
	std::ifstream file("BlockData.txt");

	// 1行ずつ読み込んで格納する
	while (std::getline(file, line)) {

		// 読み込んだデータが改行なら処理を行わない
		if (line == "")
			continue;

		// 行数で分岐
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

	// モデルの読み込み
	for (int i = 0; i < BLOCK_TYPE_AMOUNT; i++)
	{
		loadedBlockData.loadedModelHandle[i] = MV1LoadModel(loadedBlockData.loadedModelPath[i].c_str());
	}
}

// 全てのブロックに対して一括処理を行う
void STAGE::ProcessEverything(void (*func)(VECTOR pos, int num), int num) {
	VECTOR pos;
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_HEIGHT; ++j) {
			for (int k = 0; k < STAGE_WIDTH; ++k) {
				pos.x = static_cast<float>(i);
				pos.y = static_cast<float>(j);
				pos.z = static_cast<float>(k);

				// 引数で渡された処理
				(func)(pos, num);
			}
		}
	}
}

// 指定した範囲のブロックに対して一括処理を行う
void STAGE::ProcessRange(void (*func)(VECTOR pos, int num), VECTOR pos1, VECTOR pos2, int num) {
	VECTOR pos;
	for (int i = static_cast<int>(round(pos1.x)); i <= static_cast<int>(round(pos2.x)); ++i) {
		for (int j = static_cast<int>(round(pos1.y)); j <= static_cast<int>(round(pos2.y)); ++j) {
			for (int k = static_cast<int>(round(pos1.z)); k <= static_cast<int>(round(pos2.z)); ++k) {
				pos.x = static_cast<float>(i);
				pos.y = static_cast<float>(j);
				pos.z = static_cast<float>(k);

				// 引数で渡された処理
				(func)(pos, num);
			}
		}
	}
}

// ステージの初期化
void STAGE::Initialize(int startDifficulty) {
	LoadBlockData();                        // ブロックのデータを読み込む
	SetBlock(0);                            // ブロックのデータを初期化
	difficulty[0] = startDifficulty;        // ステージ開始時の難易度を設定
}

// 指定した座標がステージの範囲内か判定
bool STAGE::CheckPos(VECTOR pos) {
	return ((static_cast<int>(round(pos.x)) > -1.0f) &&
		(static_cast<int>(round(pos.y)) > -1.0f) &&
		(static_cast<int>(round(pos.z)) > -1.0f) &&
		(pos.x < STAGE_WIDTH) &&
		(pos.y < STAGE_HEIGHT) &&
		(pos.z < STAGE_WIDTH));
}

// コンストラクタ
STAGE::STAGE() {

}

// 指定した箇所のブロックを取得
BLOCK STAGE::GetBlockPlacement(VECTOR pos) {
	return blockPlacement[static_cast<int>(round(pos.x))][static_cast<int>(round(pos.y))][static_cast<int>(round(pos.z))];
}

// 指定した箇所のブロックの種類を初期化(空白ブロックにする)
void STAGE::InitBlockPlacement(VECTOR pos) {

	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(0);
}

// 指定した箇所のブロックのデータを設定
void STAGE::SetBlockPlacement(VECTOR pos, int num) {

	blockPlacement[static_cast<int>(pos.x)][static_cast<int>(pos.y)][static_cast<int>(pos.z)].SetData(num);
}