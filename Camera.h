#pragma once

#include "DxLib.h"
#include "Stage.h"

constexpr float CAMERA_ANGLE_SPEED = 2.0f;           // カメラの視点移動速度
constexpr float CAMERA_PLAYER_HEIGHT = 5.0f;         // プレイヤーの座標からどれだけ高い位置にいるか
constexpr float CAMERA_STAGE_DISTANCE = STAGE_WIDTH; // カメラとステージの距離
constexpr VECTOR POSITION_0_90_180_270[4] =          // カメラの視点ごとの座標
{
	{ (CAMERA_STAGE_DISTANCE * 2) - 1, 5.0f, STAGE_WIDTH_CENTER },
	{ STAGE_WIDTH_CENTER, 5.0f, -CAMERA_STAGE_DISTANCE },
	{ -CAMERA_STAGE_DISTANCE, 5.0f, STAGE_WIDTH_CENTER },
	{STAGE_WIDTH_CENTER, 5.0f, (CAMERA_STAGE_DISTANCE * 2) - 1 }
};

class Camera
{
private:
	VECTOR position; // 位置
	VECTOR target;   // 注視点座標
	int posNum;      // 座標番号

public:
	void Initialize(); // 初期化処理
	void Process();    // カメラの処理
	VECTOR GetPos() { return position; }  // カメラの座標を取得
	VECTOR GetTarget() { return target; } // カメラのターゲット座標を取得

	Camera();
	~Camera();
};

extern Camera camera; // カメラ