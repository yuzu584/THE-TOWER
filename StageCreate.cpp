#include <stdio.h>
#include <time.h>
#include <windows.h>
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
	FuncProcess(Random(1));
}

// 関数ポインタにセット
void CREATE_PROCESS::SetFunc(void (*newFunc)(), int num) {
	func[num] = newFunc;
}

// ミリ秒単位で乱数を生成
int CREATE_PROCESS::Random(int num) {
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	srand(tm.wMilliseconds);
	return (rand() % num);
}

// ミリ秒単位で乱数を生成(オフセットで数値をずらす)
int CREATE_PROCESS::Random(int num, int offset) {
	SYSTEMTIME tm;
    GetLocalTime(&tm);
	srand(tm.wMilliseconds);
	return ((rand() % num) + offset);
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

// 向きをランダムで指定(4方向)
void CREATE_PROCESS::SetRandDir() {

	while (true)
	{
		// 0～4の乱数を生成
		randDir = Random(4);

		// 前回の向きと同じならやり直し
		if (randDir == oldDir) {
			continue;
		}
		oldDir = randDir;

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
			stage.SetBlock(pos, 2, createProcess.GetRandDir(true));
		}
	}
}

// 一本道を生成
void TEST_CREATE_PROCESS::OneLoad() {

	// ステージ生成の向きをランダムに変更
	createProcess.SetRandDir();

	// ステージ生成の位置と向きを取得
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();

	// 一段上がるか乱数で決定
	if (createProcess.Random(16) < 8) {
		pos->y += 1;
		stage.SetBlock(*pos, -2, createProcess.GetRandDir(true));
		*pos = VAdd(*pos, *dir);
	}

	// ステージ生成
	stage.SetBlock(*pos, VAdd(*pos, VScale(*dir, 3.0f)), 1, createProcess.GetRandDir(true));
	*pos = VAdd(*pos, VScale(*dir, 3.0f));
}

// 道幅2マスの一本道を生成
void TEST_CREATE_PROCESS::OnrLoad_2Squares() {
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();
	stage.SetBlock(*pos, VAdd(*pos, VGet(1.0f, 0.0f, 5.0f)), 1, createProcess.GetRandDir(true));
}

// 正方形の床を生成
void TEST_CREATE_PROCESS::Floor() {
	pos = createProcess.GetCreationPos();
	dir = createProcess.GetCreationDir();
	int width = createProcess.Random(5);
	stage.SetBlock(*pos, VAdd(*pos, VGet(static_cast<float>(width), 0.0f, static_cast<float>(width))), 1, createProcess.GetRandDir(true));
}