#include <stdio.h>
#include "StageCreate.h"

CREATE_PROCESS createProcess;
TEST_CREATE_PROCESS test_create_process;
int TEST_CREATE_PROCESS::createCount;

// ステージ生成処理
void CREATE_PROCESS::Process() {

	// ステージ初期化処理
	test_create_process.Initialize();

	// ランダムなステージ生成処理を行う
	FuncProcess(rand() % 1);
}

// 関数ポインタにセット
void CREATE_PROCESS::SetFunc(CREATE_PROCESS_FUNC newFunc, int num) {
	func[num] = newFunc;
}

// ステージ生成処理を行う向きの番号を取得(reverse true : 向きを反転した数値 false : そのままの数値)
int CREATE_PROCESS::GetRandDir(bool reverse) {

	// 向きを反転した数値を取得するなら
	// 向きを反転した数値に加工して返す
	if (reverse) {
		randReverseDir = randDir;
		randReverseDir += 2;
		if (randReverseDir >= 4) {
			randReverseDir -= 4;
		}
		return randReverseDir;
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

		// 前回の向きの反対の向きを求める
		int reverseOldDir = (oldDir + 2);
		if (reverseOldDir > 3) {
			reverseOldDir -= 4;
		}

		// 前回の向きの反対の向き又は前回と同じ向きならやり直し
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
	createProcess.SetFunc(OneLoad, 1);
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
			stage.SetBlock(pos, 2, -1);
		}
	}
}

// 一本道を生成
void TEST_CREATE_PROCESS::OneLoad() {
	
	// ステージ生成の位置と向きを取得
	VECTOR& pos = createProcess.GetCreationPos();
	VECTOR& dir = createProcess.GetCreationDir();

	createCount = 0;
	MATRIX matrix;
	VECTOR vec1, vec2;
	int dirDiff = createProcess.GetRandDir(false); // 前回のステージ生成向きとの差

	// 踊り場を生成
	vec1 = dir;                             // ステージ生成の向きを代入
	matrix = MGetRotY(DX_PI_F / 2.0f);      // 90度回転する行列
	vec2 = VTransform(vec1, matrix);        // vec1を90度回転させた値を代入
	vec2 = VScale(vec2, ONELOAD_MAX_WIDTH); // 一本道の幅でスケール
	stage.SetBlock(pos, VAdd(pos, vec2), 1, -1);

	// ステージ生成処理が正常に行える準備が整うまで繰り返す
	while (true)
	{
		// ステージ生成の向きをランダムに変更
		createProcess.SetRandDir();

		// ステージの端までの距離を数える
		while (true) {
			if (stage.CheckPos(VAdd(pos, VScale(dir, static_cast<float>(createCount + 1))))) {
				++createCount;
			}
			else
				break;
		}

		// 端までの距離が0ブロックならやり直し
		if (createCount == 0) {
			createCount = 0;
			continue;
		}
		// 端までの距離が0ブロック以上なら抜ける
		else if (createCount > 0)
			break;
	}

	// 今回と前回のステージ生成向きの差を求める
	dirDiff -= createProcess.GetRandDir(false);
	
	// ステージ生成の向きの差によって分岐
	switch (dirDiff)
	{
	case -1:
		pos = VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH));
		break;
	case -3:
		pos = VAdd(VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH + 1)), VGet(0.0f, 0.0f, static_cast<float>(ONELOAD_MAX_WIDTH - 1)));
		break;
	case 1:
		vec1 = dir;                             // ステージ生成の向きを代入
		matrix = MGetRotY(DX_PI_F / -2.0f);     // -90度回転する行列
		vec2 = VTransform(vec1, matrix);        // vec1を-90度回転させた値を代入
		vec2 = VScale(vec2, ONELOAD_MAX_WIDTH); // 一本道の幅でスケール
		pos = VAdd(VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH)), vec2);
		break;
	case 3:
		pos = VAdd(pos, VScale(dir, ONELOAD_MAX_WIDTH));
		break;
	default:
		break;
	}
	createProcess.ClampCreationPos();

	// 前回のブロック生成処理の向きを設定
	createProcess.SetOldDir();

	// 生成するブロックの長さを決定
	createCount = rand() % createCount + 1;

	// 一本道に幅を持たせる処理で使う変数の値を設定
	vec1 = dir;                             // ステージ生成の向きを代入
	matrix = MGetRotY(DX_PI_F / 2.0f);      // 90度回転する行列
	vec2 = VTransform(vec1, matrix);        // vec1を90度回転させた値を代入
	vec2 = VScale(vec2, ONELOAD_MAX_WIDTH); // 一本道の幅でスケール

	// 一段上がるか乱数で決定、乱数が外れても設置位置の下にブロックがあれば一段上がる
	if ((rand() % RAISE_UP_RATE) || (!stage.CheckBlock(VAdd(pos, VGet(0.0f, -1.0f, 0.0f)), VAdd(VAdd(pos, vec2), VGet(0.0f, -1.0f, 0.0f))))) {

		// 一段上げる処理
		++pos.y;
		stage.SetBlock(pos, VAdd(pos, vec2), -2, createProcess.GetRandDir(true));
		pos = VAdd(pos, dir);
		createProcess.ClampCreationPos();
		--createCount;
	}

	// ステージ生成
	for (int i = 0; i < createCount; ++i) {

		// 設置位置の下にブロックがあれば一段上がる
		if (!stage.CheckBlock(VAdd(pos, VGet(0.0f, -1.0f, 0.0f)), VAdd(VAdd(pos, vec2), VGet(0.0f, -1.0f, 0.0f)))) {

			// 一段上げる処理
			++pos.y;
			stage.SetBlock(pos, VAdd(pos, vec2), -2, createProcess.GetRandDir(true));
			pos = VAdd(pos, dir);
			createProcess.ClampCreationPos();
		}
		else {
			stage.SetBlock(pos, VAdd(pos, vec2), 1, -1);
			pos = VAdd(pos, dir);
			createProcess.ClampCreationPos();
		}
	}
	createCount = 0;
}

// 穴の開いた一本道を生成
void TEST_CREATE_PROCESS::HolesOneLoad() {

}

// 正方形の床を生成
void TEST_CREATE_PROCESS::Floor() {

}

// 他の足場から切り離された独立した床を生成
void TEST_CREATE_PROCESS::FloatingFloor() {

}