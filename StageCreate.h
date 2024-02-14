#pragma once

#include "Stage.h"

constexpr int PROCESS_TYPE_AMOUNT = 5; // ステージごとの生成プロセスの数

class CREATE_PROCESS {
private:
	void (*func[PROCESS_TYPE_AMOUNT])();                      // 実行するステージ生成処理の関数ポインタ配列
	VECTOR creationPos = { 0.0f, 0.0f, 0.0f };                // ステージ生成処理を行う位置
public:
	void Process();                                           // ステージ生成処理
	void FuncProcess(int num) { (*func[num])(); }             // 指定した関数ポインタ配列の関数を実行
	void SetFunc(void (*newfunc)(), int);		              // 関数ポインタをセット
	VECTOR* GetCreationPos() { return &creationPos; }         // ステージ生成処理を行う位置を取得
};

// テストステージ生成プロセスのクラス
class TEST_CREATE_PROCESS {
public:
	void SetStageFunc();									  // ステージ生成処理の関数ポインタをセット
	void Initialize();									      // ステージ初期化
	void StartFloor();                                        // ステージ開始時の床を生成
	static void OneLoad();                                    // 一本道を生成
	static void OnrLoad_2Squares();                           // 道幅2マスの一本道を生成
};

extern CREATE_PROCESS createProcess;
extern TEST_CREATE_PROCESS test_create_process;