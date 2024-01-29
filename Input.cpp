#include "DxLib.h"
#include "Input.h"

PADINPUT input; // 入力情報を宣言

// 入力処理
void PADINPUT::Process() {

	// 1つ前のフレームの入力を保存
	int old;
	old = nowInput;

	// 現在の入力状態を取得
	nowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 今のフレームで新たに押されたボタンのビットだけ立っている値を edgeInput に代入する
	edgeInput = nowInput & ~old;
}