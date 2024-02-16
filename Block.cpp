#include "Stage.h"
#include "DrawBoard.h"

#include <string>

// ブロックのデータを代入する
void BLOCK::SetData(int num) {

	// すでにモデルが生成済みなら
	if (modelHandle != -1) {

		// モデルを削除
		MV1DeleteModel(modelHandle);

		// モデル全体のコリジョン情報のセットアップ
		MV1SetupCollInfo(modelHandle, -1);
	}

	// ブロックIDから自分のブロックのデータを探す
	int i = 0;
	while (num != loadedBlockData.loadedId[i])
	{
		if (i >= BLOCK_TYPE_AMOUNT - 1) { break; }
		++i;
	}

	// 探したブロックIDのデータを代入
	id = loadedBlockData.loadedId[i];
	name = loadedBlockData.loadedName[i];
	modelHandle = MV1DuplicateModel(loadedBlockData.loadedModelHandle[i]);
}

// ブロックのデータと向きを代入する
void BLOCK::SetData(int num, int dir) {

	// すでにモデルが生成済みなら
	if (modelHandle != -1) {

		// モデルを削除
		MV1DeleteModel(modelHandle);

		// モデル全体のコリジョン情報のセットアップ
		MV1SetupCollInfo(modelHandle, -1);
	}

	// ブロックIDから自分のブロックのデータを探す
	int i = 0;
	while (num != loadedBlockData.loadedId[i])
	{
		if (i >= BLOCK_TYPE_AMOUNT - 1) { break; }
		++i;
	}

	// 探したブロックIDのデータを代入
	id = loadedBlockData.loadedId[i];
	name = loadedBlockData.loadedName[i];
	modelHandle = MV1DuplicateModel(loadedBlockData.loadedModelHandle[i]);

	// 向きを設定
	if(dir != -1)
		SetDirection(dir);
}

// ブロックのモデルを描画
void BLOCK::Render(VECTOR pos) {
	if (modelHandle != -1) {
		MV1SetScale(modelHandle, BLOCK_SCALE);
		MV1SetPosition(modelHandle, pos);
		MV1DrawModel(modelHandle);
	}
}

// ブロックを初期化
void BLOCK::Initialize() {

	// 空気ブロックのデータを代入
	SetData(0);

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(modelHandle, -1);
}

// コンストラクタ
BLOCK::BLOCK() {

	// ブロックを初期化
	Initialize();
}

// ブロックIDを取得
int BLOCK::GetId() {
	return id;
}

// ブロック名を取得
std::string BLOCK::GetName() {
	return name;
}

// ブロックの向きを取得
int BLOCK::GetDirection() {
	return direction;
}

// ブロックの向きを指定
void BLOCK::SetDirection(int num) {
	direction = num;
	MV1SetRotationXYZ(modelHandle, BLOCK_DIRECTION[num]);
}