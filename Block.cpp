#include "Stage.h"
#include "DrawBoard.h"

#include <string>

// ブロックのデータを代入する
void Block::SetData(int num) {

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

// ブロックのモデルを描画
void Block::Render(VECTOR pos) {
	if (modelHandle != -1) {
		MV1SetScale(modelHandle, BLOCK_SCALE);
		MV1SetPosition(modelHandle, pos);
		MV1DrawModel(modelHandle);
	}
}

// コンストラクタ
Block::Block() {
	SetData(0);
}

// ブロックIDを取得
int Block::GetId() {
	return id;
}

// ブロック名を取得
std::string Block::GetName() {
	return name;
}

// ブロックの向きを取得
int Block::GetDirection() {
	return direction;
}

// ブロックの向きを指定
void Block::SetDirection(int num) {
	direction = num;
}