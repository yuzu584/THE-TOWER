#pragma once

#include "Stage.h"

constexpr int PROCESS_TYPE_AMOUNT = 5; // ステージごとの生成プロセスの数

class CREATE_PROCESS {
private:
	void (*func[PROCESS_TYPE_AMOUNT])();                      // 実行するステージ生成処理の関数ポインタ配列
	VECTOR creationPos = { 3.0f, 0.0f, 3.0f };                // ステージ生成処理を行う位置
	VECTOR creationDir = { 1.0f, 0.0f, 0.0f };                // ステージ生成処理を行う向き
public:
	void Process();                                           // ステージ生成処理
	void FuncProcess(int num) { (*func[num])(); }             // 指定した関数ポインタ配列の関数を実行
	void SetFunc(void (*newfunc)(), int);		              // 関数ポインタをセット
	int Random(int num);                                      // 乱数を生成
	int Random(int num, int offset);                          // 乱数を生成(オフセットで数値をずらす)
	void SetRandDir();                                        // 向きをランダムで指定(4方向)
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