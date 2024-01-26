#include "Camera.h"
#include "Input.h"

#include <math.h>

Camera camera; // カメラを宣言

// 初期化処理
void Camera::Initialize() {
	position = POSITION_0_90_180_270[0]; // 座標を設定
	target = VGet(5.0f, 0.0f, 5.0f);     // 注視点を設定
}

// カメラの処理
void Camera::Process() {

	// 「←」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_LEFT)
	{

	}

	// 「→」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_RIGHT)
	{

	}

	// 「↑」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_UP)
	{
		
	}

	// 「↓」ボタンが押されていた時の処理
	if (input.GetNowInput() & PAD_INPUT_DOWN)
	{

	}

	// カメラの位置と向きを設定
	SetCameraPositionAndTarget_UpVecY(position, VGet(5, 0, 5));
}

// コンストラクタ
Camera::Camera() {

	// 初期化処理
	Initialize();
}

// デストラクタ
Camera::~Camera() {

}