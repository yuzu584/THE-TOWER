#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;

// 関数ポインタにセット
void CREATE_PROCESS::SetFunc(void (*newFunc)(VECTOR*), int num) {
	func[num] = newFunc;
}

// このステージで使用するステージ生成処理の関数ポインタ配列に、ステージ生成処理の関数ポインタをセット
void TEST_CREATE_PROCESS::SetStageFunc() {
	createProcess.SetFunc(StartFloor, 0);
	createProcess.SetFunc(OneLoad, 1);
	createProcess.SetFunc(OnrLoad_2Squares, 2);
	createProcess.SetFunc(StartFloor, 3);
	createProcess.SetFunc(StartFloor, 4);
}

// ステージ初期化
void TEST_CREATE_PROCESS::Initialize() {
	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_WIDTH; ++j) {
			pos.x = static_cast<float>(i);
			pos.z = static_cast<float>(j);
			stage.SetBlockPlacement(pos, 2);
		}
	}
}

// ステージ開始時の床を生成
void TEST_CREATE_PROCESS::StartFloor(VECTOR* pos) {

}

// 一本道を生成
void TEST_CREATE_PROCESS::OneLoad(VECTOR* pos) {
	stage.SetBlockPlacement(*pos, 1);
}

// 道幅2マスの一本道を生成
void TEST_CREATE_PROCESS::OnrLoad_2Squares(VECTOR* pos) {

}