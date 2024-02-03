#pragma once
#include "DxLib.h"

constexpr float PLAYER_MOVE_SPEED = 0.01f; // 移動速度
constexpr float PLAYER_JUMP_POWER = 0.05f; // ジャンプ力
constexpr float PLAYER_GRAVITY = 0.01f; // 重力

class PLAYER
{
public:
	void Initialize(); // 初期化処理
	void Process(); // プレイヤーの処理

private:
	VECTOR position; // 座標
	int modelHandle; // モデルハンドル
};