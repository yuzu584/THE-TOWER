#include "Camera.h"
#include "Input.h"
#include "Player.h"

#include <math.h>

CAMERA camera; // カメラを宣言

// 初期化処理
void CAMERA::Initialize() {
	posNum = 1;
	target = VGet(STAGE_WIDTH_CENTER, 0.0f, STAGE_WIDTH_CENTER); // 注視点を設定
}

// カメラの処理
void CAMERA::Process() {

	// 「←」ボタンが押されていた時の処理
	if (input.GetEdgeInput() & PAD_INPUT_LEFT)
	{
		--posNum;
		if (posNum < 0)
			posNum = 3;
	}

	// 「→」ボタンが押されていた時の処理
	if (input.GetEdgeInput() & PAD_INPUT_RIGHT)
	{
		++posNum;
		if (posNum > 3)
			posNum = 0;
	}

	// 「↑」ボタンが押されていた時の処理
	if (input.GetEdgeInput() & PAD_INPUT_UP)
	{
		
	}

	// 「↓」ボタンが押されていた時の処理
	if (input.GetEdgeInput() & PAD_INPUT_DOWN)
	{
		
	}

	// カメラの位置と向きを設定
	position = POSITION_0_90_180_270[posNum];

	if ((posNum == 1) || (posNum == 3))
		position.x = player.GetPosition().x;
	if ((posNum == 0) || (posNum == 2))
		position.z = player.GetPosition().z;

	position.y = player.GetPosition().y + CAMERA_PLAYER_HEIGHT;

	target = player.GetPosition();

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