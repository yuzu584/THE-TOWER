#pragma once

#include "DxLib.h"

constexpr float CAMERA_ANGLE_SPEED = 2.0f; // カメラの視点移動速度
constexpr VECTOR POSITION_0_90_180_270[4] = // カメラの視点ごとの座標
{
	{ 15.0f, 5.0f, 5.0f },
	{ 5.0f, 5.0f, -10.0f },
	{ -10.0f, 5.0f, 5.0f },
	{ 5.0f, 5.0f, -15.0f }
};

class Camera
{
private:
	VECTOR position; // 位置
	VECTOR target;   // 注視点座標
	int posNum;      // 座標番号

public:
	void Initialize();           // 初期化処理
	void Process();              // カメラの処理

	Camera();
	~Camera();
};

extern Camera camera; // カメラ