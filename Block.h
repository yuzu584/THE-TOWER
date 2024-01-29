#pragma once

#include "DxLib.h"
#include <string>
#include <fstream>

constexpr int BLOCK_TYPE_AMOUNT = 4;                 // ブロックの種類の数
constexpr VECTOR BLOCK_SCALE = { 0.5f, 0.5f, 0.5f }; // ブロックのスケール

class Block {
private:
	int id = 0;                   // ブロックID
	std::string name = "";        // 名前
	int direction = 0;            // 向き(0:前 1:右 2:後ろ 3:左 4:上 5:下)
	int modelHandle = 0;          // モデルハンドル
public:
	void SetData(int num);   // ブロックのデータを代入する
	void Render(VECTOR pos); // ブロックのモデルを描画

	Block();

	int GetId();
	std::string GetName();
	int GetDirection();
	void SetDirection(int num);
};