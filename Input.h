#pragma once

// 入力情報のクラス
class PADINPUT {
private:
	int nowInput;  // 現在の入力値
	int edgeInput; // 現在のフレームで押されたボタンのみビットが立っている入力値
public:
	void Process();                        // 入力処理
	int GetNowInput() { return nowInput; } // 現在の入力値を取得
	int GetEdgeInput() { return edgeInput; } // 現在のフレームで押されたボタンの入力値を取得
};

extern PADINPUT input; // 入力情報