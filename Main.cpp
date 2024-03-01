#include "DxLib.h"
#include "Stage.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "StageCreate.h"
#include <windows.h>
#include <algorithm>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);

	// 画面サイズを設定
	SetGraphMode(800, 600, 16);

	// ライブラリの初期化
	if (DxLib_Init() == -1) return -1;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラのクリッピング距離を設定
	SetCameraNearFar(0.1f, 300.0f);

	// 背景の色を設定
	SetBackgroundColor(128, 128, 128);

	stage.Initialize(0);
	stage.Create();

	player.Initialize();

	// 標準ライトの方向をY軸のマイナス方向にする
	SetLightDirection(VGet(0.5f, -1.0f, 0.5f));

	// 乱数をミリ秒単位で生成
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	srand(tm.wMilliseconds);

	// ESCキーが押されるか、ウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 現在のカウントを取得する
		int time = GetNowCount();

		// 画面をクリア
		ClearDrawScreen();
		
		// 入力処理
		input.Process();

		// カメラの処理
		camera.Process();

		// ステージの処理
		stage.Process();

		// プレイヤーの処理
		player.Process();

		// 裏画面の内容を表画面に反映
		ScreenFlip();

		// 17ミリ秒(約秒間60フレームだった時の1フレームあたりの経過時間)
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