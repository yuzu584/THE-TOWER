#include <stdio.h>
#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;
VECTOR* TEST_CREATE_PROCESS::pos;
VECTOR* TEST_CREATE_PROCESS::dir;

// ステージ生成処理
void CREATE_PROCESS::Process() {

	// ステージ初期化処理
	test_create_process.Initialize();

	// 現在時刻をもとに初期化して乱数を生成し、
	// ランダムなステージ生成処理を行う
	FuncProcess(rand() % 1);
}

// 関数ポインタにセット
void CREATE_PROCESS::SetFunc(void (*newFunc)(), int num) {
	func[num] = newFunc;
}

// ステージ生成処理を行う向きの番号を取得(reverse true : 向きを反転した数値 false : そのままの数値)
int CREATE_PROCESS::GetRandDir(bool reverse) {

	// 向きを反転した数値を取得するなら
	// 向きを反転した数値に加工して返す
	if (reverse) {
		randDir += 2;
		if (randDir >= 4) {
			randDir -= 4;
		}
	}

	return randDir;
}

// ステージ生成処理を行う位置の数値を範囲内に収める
void CREATE_PROCESS::ClampCreationPos() {
	if (creationPos.x < 0)
		creationPos.x = 0;
	if (creationPos.y < 0)
		creationPos.y = 0;
	if (creationPos.z < 0)
		creationPos.z = 0;

	if (creationPos.x > STAGE_WIDTH - 1)
		creationPos.x = STAGE_WIDTH - 1;
	if (creationPos.y > STAGE_HEIGHT - 1)
		creationPos.y = STAGE_HEIGHT - 1;
	if (creationPos.z > STAGE_WIDTH - 1)
		creationPos.z = STAGE_WIDTH - 1;
}

// 向きをランダムで指定(4方向)
void CREATE_PROCESS::SetRandDir() {

	while (true)
	{
		// 0～4の乱数を生成
		randDir = rand() % 4;

		// 前回の向きの反対の向き又は前回と同じ向きならやり直し
		int reverseOldDir = oldDir += 2;
		if (reverseOldDir > 3) {
			reverseOldDir -= 4;
		}
		if ((randDir == reverseOldDir) || (randDir == oldDir)) {
			continue;
		}

		// 生成した乱数によって分岐
		switch (randDir)
		{
		case 0:
			creationDir = { 1.0f, 0.0f, 0.0f };
			break;
		case 1:
			creationDir = { 0.0f, 0.0f, -1.0f };
			break;
		case 2:
			creationDir = { -1.0f, 0.0f, 0.0f };
			break;
		case 3:
			creationDir = { 0.0f, 0.0f, 1.0f };
			break;
		default:
			break;
		}

		// 指定した向きのブロックがステージの範囲外ならやり直し
		if (!stage.CheckPos(VAdd(creationPos, creationDir)))
			continue;

		break;
	}
}

// このステージで使用するステージ生成処理の関数ポインタ配列に、ステージ生成処理の関数ポインタをセット
void TEST_CREATE_PROCESS::SetStageFunc() {
	createProcess.SetFunc(OneLoad, 0);
	createProcess.SetFunc(OnrLoad_2Squares, 1);
	createProcess.SetFunc(Floor, 2);
	createProcess.SetFunc(Floor, 3);
	createProcess.SetFunc(Floor, 4);
}

// ステージ初期化
void TEST_CREATE_PROCESS::Initialize() {

	// ステージ生成の位置と向きを取得
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();

	// ステージ開始時の床を生成
	StartFloor();

	// このステージで使用するステージ生成処理の関数ポインタ配列に、ステージ生成処理の関数ポインタをセット
	SetStageFunc();

#if 0
	// 最初の階段を設置
	++pos->y;
	createProcess.ClampCreationPos();
	stage.SetBlock(*pos, -2, createProcess.GetRandDir(true));
	*pos = VAdd(*pos, *dir);
	createProcess.ClampCreationPos();
#endif // 0
}

// ステージ開始時の床を生成
void TEST_CREATE_PROCESS::StartFloor() {

	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < STAGE_WIDTH; ++i) {
		for (int j = 0; j < STAGE_WIDTH; ++j) {
			pos.x = static_cast<float>(i);
			pos.z = static_cast<float>(j);
			stage.SetBlock(pos, 2, -1);
		}
	}
}

// 一本道を生成
void TEST_CREATE_PROCESS::OneLoad() {

	// ステージ生成の向きをランダムに変更
	createProcess.SetRandDir();

	int createCount = 0;

	// ステージの端までの距離を数える
	while (true) {
		if (stage.CheckPos(VAdd(*pos, VScale(*dir, static_cast<float>(createCount))))) {
			++createCount;
		}
		else
			break;
	}

	// 生成するブロックの長さを決定
	if (createCount > 7)
		createCount = 7;
	createCount = rand() % createCount + 1;

	// 一段上がるか乱数で決定、乱数が外れても設置位置の下にブロックがあれば一段上がる
	if ((rand() % 16 < 4) || (!stage.CheckBlock(VAdd(*pos, VGet(0.0f, -1.0f, 0.0f))))) {

		// 一段上げる処理
		++pos->y;
		createProcess.ClampCreationPos();
		stage.SetBlock(*pos, -2, createProcess.GetRandDir(true));
		*pos = VAdd(*pos, *dir);
		createProcess.ClampCreationPos();
		--createCount;
	}

	// ステージ生成
	stage.SetBlock(*pos, VAdd(*pos, VScale(*dir, static_cast<float>(createCount))), 1, -1);
	*pos = VAdd(*pos, VScale(*dir, static_cast<float>(createCount)));
	createProcess.ClampCreationPos();
	createCount = 0;
}

// 道幅2マスの一本道を生成
void TEST_CREATE_PROCESS::OnrLoad_2Squares() {

}

// 正方形の床を生成
void TEST_CREATE_PROCESS::Floor() {

}