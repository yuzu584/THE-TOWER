#pragma once
#include "DxLib.h"

constexpr float PLAYER_MOVE_SPEED        = 1.0f; // 移動速度
constexpr float PLAYER_JUMP_POWER        = 1.0f; // ジャンプ力
constexpr float PLAYER_GRAVITY           = 1.0f; // 重力
constexpr float PLAYER_ANGLE_SPEED       = 0.2f; // 角度変化速度
constexpr int   PLAYER_MAX_HITCOLL       = 1024; // 処理するコリジョンポリゴンの最大数
constexpr float PLAYER_ENUM_DEFAULT_SIZE = 3.0f; // 周囲のポリゴン検出に使用する球の初期サイズ
constexpr float PLAYER_HIT_WIDTH         = 0.5f; // 当たり判定カプセルの半径
constexpr float PLAYER_HIT_HEIGHT        = 0.5f; // 当たり判定カプセルの高さ
constexpr int   PLAYER_HIT_TRYNUM        = 16;   // 壁押し出し処理の最大試行回数
constexpr float PLAYER_HIT_SLIDE_LENGTH  = 0.1f; // 一度の壁押し出し処理でスライドさせる距離

enum AnimeState {
	Neutral = 1,
	Run = 2,
	Jump = 3,
};

class Player {
private:
	VECTOR position; // 座標
	VECTOR targetDirection; // 向く方向のベクトル
	float angle;            // 向いている角度
	float speed_y;          // y軸の速度
	int modelHandle; // モデルハンドル
	VECTOR moveVector_H; // プレイヤーの左右の移動ベクトル
	VECTOR moveVector_V; // プレイヤーの前後の移動ベクトル
	VECTOR moveVector;   // 最終的な移動ベクトル
	bool moveFlag; // 移動したかどうか
	AnimeState state; // 状態
public:
	void Initialize(); // プレイヤーの初期化
	void Process(); // プレイヤーの処理
	void Move(VECTOR moveVector); // プレイヤーの移動処理
	void AngleProcess(); // プレイヤーの向きを変える処理
	void AnimProcess(); // プレイヤーのアニメーション処理
	Player();
	~Player();
};

extern Player player;