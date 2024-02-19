#pragma once

#include "Stage.h"

constexpr int PROCESS_TYPE_AMOUNT = 5; // ステージごとの生成プロセスの数

class CREATE_PROCESS {
private:
	void (*func[PROCESS_TYPE_AMOUNT])();                      // 実行するステージ生成処理の関数ポインタ配列
	VECTOR creationPos = { 3.0f, 0.0f, 3.0f };                // ステージ生成処理を行う位置
	VECTOR creationDir = { 1.0f, 0.0f, 0.0f };                // ステージ生成処理を行う向き
	int randDir;                                              // ステージ生成処理を行う向きの番号
	int oldDir = 1;                                           // 前回のステージ生成処理を行う向きの番号
public:
	void Process();                                           // ステージ生成処理
	void FuncProcess(int num) { (*func[num])(); }             // 指定した関数ポインタ配列の関数を実行
	void SetFunc(void (*newfunc)(), int);		              // 関数ポインタをセット
	void ClampCreationPos();                                  // ステージ生成処理を行う位置の数値を範囲内に収める
	void SetRandDir();                                        // 向きをランダムで指定(4方向)
	void SetOldDir() { oldDir = randDir; }                    // 前回のステージ生成処理を行う向きの番号をセット
	int GetRandDir(bool reverse);                             // ステージ生成処理を行う向きの番号を取得(reverse true : 向きを反転した数値 false : そのままの数値)
	VECTOR* GetCreationPos() { return &creationPos; }         // ステージ生成処理を行う位置を取得
	VECTOR* GetCreationDir() { return &creationDir; }         // ステージ生成処理を行う向きを取得
};

// テストステージ生成プロセスのクラス
class TEST_CREATE_PROCESS {
private:
	static VECTOR* pos;                                       // ステージ生成処理を行う位置を代入する変数
	static VECTOR* dir;                                       // ステージ生成処理を行う向きを代入する変数
public:
	void SetStageFunc();									  // ステージ生成処理の関数ポインタをセット
	void Initialize();									      // ステージ初期化
	void StartFloor();                                        // ステージ開始時の床を生成
	static void OneLoad();                                    // 一本道を生成
	static void OnrLoad_2Squares();                           // 道幅2マスの一本道を生成
	static void Floor();                                      // 正方形の床を生成
};

extern CREATE_PROCESS createProcess;
extern TEST_CREATE_PROCESS test_create_process;