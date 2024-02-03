#include "Player.h"
#include "Camera.h"

// 初期化処理
void PLAYER::Initialize() {

	// 座標を設定
	position = VGet(0.0f, 1.0f, 0.0f);

	// モデル読み込み
	modelHandle = MV1LoadModel("BlockModel/WoodBox.mv1");
}

// プレイヤーの処理
void PLAYER::Process() {
	VECTOR UpMoveVec;   // 上入力時の移動方向
	VECTOR LeftMoveVec; // 左入力時の移動方向
	VECTOR moveVec;     // 最終的な移動方向
	int moveFlag;       // 移動したかどうか( true : 移動した  false : 移動していない )

	// 移動方向ベクトルを算出
	// 上方向の移動ベクトルはカメラの視線方向からY成分を抜いたもの
	UpMoveVec = VSub(camera.GetTarget(), camera.GetPos());
	UpMoveVec.y = 0.0f;

	// 左方向の移動ベクトルは上方向ベクトルとY軸のプラス方向のベクトルに垂直な方向
	LeftMoveVec = VCross(UpMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// 二つのベクトルを正規化
	UpMoveVec = VNorm(UpMoveVec);
	LeftMoveVec = VNorm(LeftMoveVec);
}