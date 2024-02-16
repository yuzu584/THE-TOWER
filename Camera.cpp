#include "Camera.h"
#include "Input.h"
#include "Player.h"

#include <math.h>

CAMERA camera; // カメラを宣言

// 初期化処理
void CAMERA::Initialize() {
	position = POSITION_0_90_180_270[1]; // 座標を設定
	target = VGet(STAGE_WIDTH_CENTER, 0.0f, STAGE_WIDTH_CENTER); // 注視点を設定
}

// カメラの処理
void CAMERA::Process() {

	// 「←」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_LEFT)
	{
		position = POSITION_0_90_180_270[2]; // 座標を設定
	}

	// 「→」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_RIGHT)
	{
		position = POSITION_0_90_180_270[0]; // 座標を設定
	}

	// 「↑」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_UP)
	{
		position = POSITION_0_90_180_270[3]; // 座標を設定
	}

	// 「↓」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_DOWN)
	{
		position = POSITION_0_90_180_270[1]; // 座標を設定
	}

	// カメラの位置と向きを設定
	SetCameraPositionAndTarget_UpVecY(position, target);
}

// コンストラクタ
CAMERA::CAMERA() {

	// 初期化処理
	Initialize();
}

// デストラクタ
CAMERA::~CAMERA() {

}