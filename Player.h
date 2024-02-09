#pragma once
#include "DxLib.h"

constexpr float PLAYER_MOVE_SPEED = 0.05f; // 移動速度
constexpr float PLAYER_JUMP_POWER = 0.11f; // ジャンプ力
constexpr float PLAYER_GRAVITY = 0.005f; // 重力
constexpr float PLAYER_ANGLE_SPEED = 0.2f; // 角度を変える速度
constexpr int  PLAYER_MAX_HITCOLL = 2048; // 処理するコリジョンポリゴンの最大数
constexpr float PLAYER_COLL_SPHERE_SIZE = 3.0f;	// 周囲のポリゴン検出に使用する球のサイズ
constexpr float PLAYER_HIT_WIDTH = 0.25f;		// 当たり判定カプセルの半径
constexpr float PLAYER_HIT_HEIGHT = 0.25f;		// 当たり判定カプセルの高さ
constexpr int PLAYER_HIT_TRYNUM = 16;		// 壁押し出し処理の最大試行回数
constexpr float PLAYER_HIT_SLIDE_LENGTH = 0.02f;		// 一度の壁押し出し処理でスライドさせる距離

enum STATE
{
	neutral = 0,
	run = 1,
	jump = 2,
};

class PLAYER
{
public:
	void Initialize(); // 初期化処理
	void Process();    // プレイヤーの処理
	void AngleProcess(); // 向きの処理
	void Move(VECTOR moveVec);         // 移動処理
	VECTOR GetPosition() { return position; }

private:
	VECTOR position; // 座標
	int modelHandle; // モデルハンドル
	STATE state;     // 動作の状態
	float speed_y;   // Y軸の移動速度
	float angle;     // 向き
	VECTOR targetDirection; // 向く対象の座標
};

extern PLAYER player;