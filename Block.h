#pragma once

#include "DxLib.h"
#include <string>
#include <fstream>

constexpr int BLOCK_TYPE_AMOUNT = 6;                 // ブロックの種類の数
constexpr VECTOR BLOCK_SCALE = { 0.5f, 0.5f, 0.5f }; // ブロックのスケール
constexpr VECTOR BLOCK_DIRECTION[4] = {              // ブロックの角度の配列
	{0.0f, 270.0f * DX_PI_F / 180.0f, 0.0f}, // 右
	{0.0f, 0.0f, 0.0f},                      // 前
	{0.0f, 90.0f * DX_PI_F / 180.0f, 0.0f},  // 左
	{0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f}, // 後ろ
};

class BLOCK {
private:
	int id = 0;                   // ブロックID
	std::string name = "";        // 名前
	int direction = 0;            // 向き(0:右 1:前 2:左 3:後ろ)
	int modelHandle = 0;          // モデルハンドル
public:
	void SetData(int num);   // ブロックのデータを代入する
	void Render(VECTOR pos); // ブロックのモデルを描画
	void Initialize(); // ブロックを初期化

	BLOCK();

	int GetId();
	std::string GetName();
	int GetDirection();
	void SetDirection(int num);
	int GetModelHandle() { return modelHandle; } // モデルハンドルを取得
};