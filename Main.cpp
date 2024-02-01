#include "DxLib.h"
#include "Stage.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);

	// ライブラリの初期化
	if (DxLib_Init() == -1) return -1;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラのクリッピング距離を設定
	SetCameraNearFar(0.1f, 300.0f);

	// 背景の色を設定
	SetBackgroundColor(128, 128, 128);

	player = new Player();

	stage.Initialize(0);

	// ESCキーが押されるか、ウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 現在のカウントを取得する
		int time = GetNowCount();

		// 画面をクリア
		ClearDrawScreen();

		// 入力処理
		input.Process();

		// プレイヤーの処理
		player->Process();

		// カメラの処理
		camera.Process();

		// ステージを描画
		stage.Render();

		// プレイヤーモデルの描画
		int m = player->GetModelHandle();
		MV1DrawModel(m);

		// 裏画面の内容を表画面に反映
		ScreenFlip();

		// １７ミリ秒(約秒間６０フレームだった時の１フレームあたりの経過時間)
		// 経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
		}
	}

	// ライブラリの後始末
	DxLib_End();

	// ソフト終了
	return 0;
}