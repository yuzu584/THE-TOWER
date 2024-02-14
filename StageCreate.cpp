#include <stdio.h>
#include <time.h>
#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;

// ステージ生成処理
void CREATE_PROCESS::Process() {

	// ステージ初期化処理
	test_create_process.Initialize();

	// 現在時刻をもとに初期化して乱数を生成し、
	// ランダムなステージ生成処理を行う
	srand((unsigned int)time(NULL));
	FuncProcess(rand() % PROCESS_TYPE_AMOUNT);
}

// 関数ポインタにセット
void CREATE_PROCESS::SetFunc(void (*newFunc)(), int num) {
	func[num] = newFunc;
}

// このステージで使用するステージ生成処理の関数ポインタ配列に、ステージ生成処理の関数ポインタをセット
void TEST_CREATE_PROCESS::SetStageFunc() {
	createProcess.SetFunc(OneLoad, 0);
	createProcess.SetFunc(OnrLoad_2Squares, 1);
	createProcess.SetFunc(OneLoad, 2);
	createProcess.SetFunc(OnrLoad_2Squares, 3);
	createProcess.SetFunc(OneLoad, 4);
}

// ステージ初期化
void TEST_CREATE_PROCESS::Initialize() {

	// ステージ開始時の床を生成
	StartFloor();

	// このステージで使用するステージ生成処理の関数ポインタ配列に、ステージ生成処理の関数ポインタをセット
	SetStageFunc();
}

// ステージ開始時の床を生成
void TEST_CREATE_PROCESS::StartFloor() {

	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_WIDTH; ++j) {
			pos.x = static_cast<float>(i);
			pos.z = static_cast<float>(j);
			stage.SetBlockPlacement(pos, 2);
		}
	}
}

// 一本道を生成
void TEST_CREATE_PROCESS::OneLoad() {
	VECTOR* pos = createProcess.GetCreationPos();
	stage.SetBlock_Range(VGet(0.0f, 0.0f, 0.0f), VGet(1.0f, 1.0f, 1.0f), 1);
}

// 道幅2マスの一本道を生成
void TEST_CREATE_PROCESS::OnrLoad_2Squares() {
	VECTOR* pos = createProcess.GetCreationPos();
	stage.SetBlock_Range(VGet(0.0f, 0.0f, 0.0f), VGet(3.0f, 1.0f, 3.0f), -1);
}