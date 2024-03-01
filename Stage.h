#pragma once

#include "Block.h"

constexpr int STAGE_WIDTH = 10;                                                   // ステージの幅(x, z)
constexpr int STAGE_HEIGHT = 120;                                                 // ステージの高さ
constexpr int STAGE_DIFFICULTY_SPACE = 30;                                        // 難易度が変わる間隔(y座標)
constexpr float STAGE_WIDTH_CENTER = ((static_cast<float>(STAGE_WIDTH) - 1) / 2); // ステージの幅の中央
constexpr float STAGE_CREATE_HEIGHT_TO_PLAYER = 2.0f;                             // ステージ生成の際にプレイヤーの位置から何ブロック高い位置を生成位置にするか

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

class STAGE {
private:
	static BLOCK blockPlacement[STAGE_WIDTH][STAGE_HEIGHT][STAGE_WIDTH];             // ブロックの配置
	int difficulty[STAGE_HEIGHT / STAGE_DIFFICULTY_SPACE];                           // ステージの難易度
	void (*func)(VECTOR, int, int);                                                  // 戻り値のない関数ポインタの定義
	static void RenderFunc(VECTOR pos, int num, int dir);                            // ステージの描画(関数ポインタで指定)
	static void SetBlockFunc(VECTOR pos, int num, int dir);                          // ブロックにデータをセット(関数ポインタで指定)
public:
	void Process();                                                                  // ステージの処理
	void Create();                                                                   // ステージの生成
	void Render();                                                                   // ステージの描画
	void SetBlock(int num, int dir);                                                 // ブロックをセット(全て)
	void SetBlock(VECTOR pos, int num, int dir);                                     // ブロックをセット
	void SetBlock(VECTOR pos1, VECTOR pos2, int num, int dir);                       // ブロックをセット(範囲)
	void LoadBlockData();                                                            // ブロックのデータを読み込む
	void ProcessEverything(void (*func)(VECTOR, int, int), int num, int dir);                      // 全てのブロックに対して一括処理を行う
	void ProcessRange(void (*func)(VECTOR, int, int), VECTOR pos1, VECTOR pos2, int num, int dir); // 指定した範囲のブロックに対して一括処理を行う
	void Initialize(int startDifficulty);											 // ステージの初期化
	bool CheckPos(VECTOR pos);                                                       // 指定した座標がステージの範囲内か判定
	bool CheckPos(VECTOR pos1, VECTOR pos2);                                         // 指定した座標がステージの範囲内か判定(範囲)
	bool CheckBlock(VECTOR pos);                                                     // 指定した座標にブロックが存在するか判定
	bool CheckBlock(VECTOR pos1, VECTOR pos2);                                       // 指定した座標にブロックが存在するか判定(範囲)

	STAGE();

	BLOCK GetBlockPlacement(VECTOR pos); // 指定した箇所のブロックを取得
	void InitBlockPlacement(VECTOR pos);
};

extern STAGE stage;