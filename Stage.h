#pragma once

#include "Block.h"

constexpr int STAGE_WIDTH = 10;                                                   // ステージの幅(x, z)
constexpr int STAGE_HEIGHT = 120;                                                 // ステージの高さ
constexpr int STAGE_DIFFICULTY_SPACE = 30;                                        // 難易度が変わる間隔(y座標)
constexpr float STAGE_WIDTH_CENTER = ((static_cast<float>(STAGE_WIDTH) - 1) / 2); // ステージの幅の中央

// 読み込んだブロックのデータの構造体
struct LoadedBlockData
{
	// 読み込んだブロックIDの配列
	int loadedId[BLOCK_TYPE_AMOUNT];

	// 読み込んだブロック名の配列
	std::string loadedName[BLOCK_TYPE_AMOUNT];

	// 読み込んだモデルのパスの配列
	std::string loadedModelPath[BLOCK_TYPE_AMOUNT];

	// 読み込んだモデルハンドルの配列
	int loadedModelHandle[BLOCK_TYPE_AMOUNT];
};

extern LoadedBlockData loadedBlockData;

class Stage {
private:
	static Block blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH]; // ブロックの配置
	int difficulty[STAGE_HEIGHT / STAGE_DIFFICULTY_SPACE];               // ステージの難易度
	void (*func)(VECTOR);                                                // 戻り値のない関数ポインタの定義
	static void RenderFunc(VECTOR pos);                                  // ステージの描画(関数ポインタで指定)
	static void SetDataInBlockFunc(VECTOR pos);                          // ブロックにデータをセット(関数ポインタで指定)
public:
	void Create();                                                       // ステージの生成
	void Render();                                                       // ステージの描画
	void SetDataInBlock();                                               // ブロックにデータをセット
	void LoadBlockData();                                                // ブロックのデータを読み込む
	void ProcessEverything(void (*func)(VECTOR pos));                    // 全てのブロックに対して処理を行う

	Stage(int startDifficulty);

	int GetBlockPlacement(VECTOR pos);
	void InitBlockPlacement(VECTOR pos);
	void SetBlockPlacement(VECTOR pos, int num);
};