#include "Player.h"
#include "Camera.h"
#include "Input.h"

PLAYER player;

// 初期化処理
void PLAYER::Initialize() {

	// 座標を設定
	position = VGet(4.5f, 1.0f, 4.5f);

	// モデル読み込み
	modelHandle = MV1LoadModel("BlockModel/WoodBox.mv1");

	// スケールを設定
	MV1SetScale(modelHandle, VGet(0.5f, 0.5f, 0.5f));
}

// プレイヤーの処理
void PLAYER::Process() {
	VECTOR upMoveVec;   // 上入力時の移動方向
	VECTOR leftMoveVec; // 左入力時の移動方向
	VECTOR moveVec;     // 最終的な移動方向
	int moveFlag;       // 移動したかどうか( true : 移動した  false : 移動していない )

	// 移動方向ベクトルを算出
	// 上方向の移動ベクトルはカメラの視線方向からY成分を抜いたもの
	upMoveVec = VSub(camera.GetTarget(), camera.GetPos());
	upMoveVec.y = 0.0f;

	// 左方向の移動ベクトルは上方向ベクトルとY軸のプラス方向のベクトルに垂直な方向
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// 二つのベクトルを正規化
	upMoveVec = VNorm(upMoveVec);
	leftMoveVec = VNorm(leftMoveVec);

	// このフレームの移動ベクトルを初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	// 移動していない状態にする
	moveFlag = false;

	// 左入力されたらカメラから見て左方向に移動
	if (input.GetNowInput() & PAD_INPUT_4)
	{
		// 移動ベクトルに左入力時の移動ベクトルを加算
		moveVec = VAdd(moveVec, leftMoveVec);

		// 移動フラグを立てる
		moveFlag = 1;
	}
	else
		// 右入力されたらカメラから見て右方向に移動
		if (input.GetNowInput() & PAD_INPUT_6)
		{
			// 移動ベクトルに左入力時の移動ベクトルを反転したものを加算
			moveVec = VAdd(moveVec, VScale(leftMoveVec, -1.0f));

			// 移動フラグを立てる
			moveFlag = 1;
		}

	// 上入力されたらカメラの見ている方向に移動
	if (input.GetNowInput() & PAD_INPUT_8)
	{
		// 移動ベクトルに上入力時の移動ベクトルを加算
		moveVec = VAdd(moveVec, upMoveVec);

		// 移動フラグを立てる
		moveFlag = 1;
	}
	else
		// 下入力されたらカメラの方向に移動する
		if (input.GetNowInput() & PAD_INPUT_5)
		{
			// 上入力時の移動ベクトルを反転したものを加算
			moveVec = VAdd(moveVec, VScale(upMoveVec, -1.0f));

			// 移動フラグを立てる
			moveFlag = 1;
		}

	// プレイヤーの状態が「ジャンプ」ではなく、且つジャンプボタンが押されていたらジャンプ
	if (state != 2 && (input.GetEdgeInput() & PAD_INPUT_A))
	{
		// 状態を「ジャンプ」にする
		state = jump;

		// Y軸方向の速度をセット
		speed_y = PLAYER_JUMP_POWER;
	}

	// 移動しているかどうかで分岐
	if (moveFlag)
	{
		// 移動ベクトルを正規化したものをプレイヤーが向く方向として設定
		targetDirection = VNorm(moveVec);

		// プレイヤーが向くべき方向ベクトルをプレイヤーのスピード倍したものを移動ベクトルとする
		moveVec = VScale(targetDirection, PLAYER_MOVE_SPEED);

		// 「立ち」状態なら
		if (state == 0)
		{
			// 状態を「走り」にする
			state = run;
		}
	}
	else
	{
		// このフレームで移動していない、且つ状態が「走り」なら
		if (state == 1)
		{
			// 状態を「立ち」にする
			state = neutral;
		}
	}

	// Y軸方向の速度を重力分減算する
	speed_y -= PLAYER_GRAVITY;

	// 移動ベクトルのY成分をY軸方向の速度にする
	moveVec.y = speed_y;

	// プレイヤーの移動方向にモデルの方向を近づける
	AngleProcess();

	// コリジョンを考慮しつつプレイヤーを移動
	Move(moveVec);
}

// 向きの処理
void PLAYER::AngleProcess() {
	float targetAngle;	// 目標角度
	float angleDiff;	// 目標角度と現在の角度との差

	// 目標の方向ベクトルから角度を算出
	targetAngle = atan2(targetDirection.x, targetDirection.z);

	// 目標の角度と現在の角度との差を割り出す
	{
		// 目標角度と現在の角度との差を計算
		angleDiff = targetAngle - angle;

		// 差が180度以上になっていたら修正
		if (angleDiff < -DX_PI_F)
		{
			angleDiff += DX_TWO_PI_F;
		}
		else
			if (angleDiff > DX_PI_F)
			{
				angleDiff -= DX_TWO_PI_F;
			}
	}

	// 角度の差を0に近づける
	if (angleDiff > 0.0f)
	{
		// 差がプラスの場合は引く
		angleDiff -= PLAYER_ANGLE_SPEED;
		if (angleDiff < 0.0f)
		{
			angleDiff = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		angleDiff += PLAYER_ANGLE_SPEED;
		if (angleDiff > 0.0f)
		{
			angleDiff = 0.0f;
		}
	}

	// モデルの角度を更新
	angle = targetAngle - angleDiff;
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

// 移動処理
void PLAYER::Move(VECTOR moveVec) {
	int i, j, k;										// 汎用カウンタ変数
	int moveFlag;										// 水平方向に移動したかどうかのフラグ( 0:移動していない  1:移動した )
	int hitFlag;										// ポリゴンに当たったかどうかを記憶する変数( 0:当たっていない  1:当たった )
	MV1_COLL_RESULT_POLY_DIM hitDim[7];					// プレイヤーの周囲のポリゴンを検出した結果が代入される当たり判定結果構造体
	int hitDimNum;										// HitDim の有効な配列要素数
	int wallNum;										// 壁ポリゴンと判断されたポリゴンの数
	int floorNum;										// 床ポリゴンと判断されたポリゴンの数
	MV1_COLL_RESULT_POLY* wall[PLAYER_MAX_HITCOLL];		// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* floor[PLAYER_MAX_HITCOLL];	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* poly;							// ポリゴンの構造体にアクセスするために使用するポインタ
	HITRESULT_LINE lineRes;								// 線分とポリゴンとの当たり判定の結果を代入する構造体
	VECTOR oldPos;										// 移動前の座標	
	VECTOR nowPos;										// 移動後の座標

	// 移動前の座標を保存
	oldPos = position;

	// 移動後の座標を算出
	nowPos = VAdd(position, moveVec);

	// 取得するブロックの座標
	VECTOR blockPos[7] = {
		{nowPos},									// プレイヤーの座標
		{VAdd(nowPos, VGet(1.0f, 0.0f, 0.0f))},		// X軸にプラスのブロックの座標
		{VAdd(nowPos, VGet(0.0f, 0.0f, -1.0f))},	// Z軸にマイナスのブロックの座標
		{VAdd(nowPos, VGet(-1.0f, 0.0f, 0.0f))},	// X軸にマイナスのブロックの座標
		{VAdd(nowPos, VGet(0.0f, 0.0f, 1.0f))},		// Z軸にプラスのブロックの座標
		{VAdd(nowPos, VGet(0.0f, 1.0f, 0.0f))},		// Y軸にプラスのブロックの座標
		{VAdd(nowPos, VGet(0.0f, -1.0f, 0.0f))},	// Y軸にマイナスのブロックの座標
	};

	// プレイヤーの周囲のブロックのポリゴンを取得
	for ( i = 0; i < 7; ++i)
	{
		if (stage.CheckPos(blockPos[i])) {
			hitDim[i] = MV1CollCheck_Sphere(stage.GetBlockPlacement(blockPos[i]).GetModelHandle(), -1, position, PLAYER_COLL_SPHERE_SIZE + VSize(moveVec));
		}
		else hitDim[i].Dim = NULL;
	}

	// hitDimの有効な数は検出したブロックの数
	hitDimNum = 7;

	// x軸かz軸に 0.01f 以上移動した場合は移動フラグを立てる
	if (fabs(moveVec.x) > 0.01f || fabs(moveVec.z) > 0.01f)
	{
		moveFlag = true;
	}
	else
	{
		moveFlag = false;
	}

	// 検出したポリゴンが壁ポリゴンか床ポリゴンかを判断
	// 変数を初期化
	wallNum = 0;
	floorNum = 0;

	// 検出したブロックのポリゴンの数繰り返し
	for (j = 0; j < hitDimNum; ++j) {
		for (i = 0; i < hitDim[j].HitNum; ++i) {

			if (hitDim[i].Dim != NULL) {

				// 壁ポリゴンか判断(ポリゴンの法線のY成分が限りなく0に近いかで判断)
				if (hitDim[j].Dim[i].Normal.y < 0.000001f && hitDim[j].Dim[i].Normal.y > -0.000001f) {

					// 壁ポリゴンの場合でも、プレイヤーの足元のY座標+0.1fより高いポリゴンのみ当たり判定を行う
					if (hitDim[j].Dim[i].Position[0].y > position.y + 0.1f ||
						hitDim[j].Dim[i].Position[1].y > position.y + 0.1f ||
						hitDim[j].Dim[i].Position[2].y > position.y + 0.1f) {

						// ポリゴンの数が列挙できる限界数に達していなければポリゴンを配列に追加
						if (wallNum < PLAYER_MAX_HITCOLL) {

							// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存
							wall[wallNum] = &hitDim[j].Dim[i];

							// 壁ポリゴンの数を加算
							++wallNum;
						}
					}
				}
				else {

					// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
					if (floorNum < PLAYER_MAX_HITCOLL) {

						// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
						floor[floorNum] = &hitDim[j].Dim[i];

						// 床ポリゴンの数を加算
						++floorNum;
					}
				}
			}
		}
	}

	// 壁ポリゴンとの当たり判定処理
	if (wallNum != 0) {

		// 壁に当たったかどうかのフラグを折る
		hitFlag = false;

		// 移動したかどうかで処理を分岐
		if (moveFlag == true) {

			// 壁ポリゴンの数繰り返す
			for ( i = 0; i < wallNum; ++i)
			{
				// i番目の壁ポリゴンのアドレスを取得
				poly = wall[i];

				// ポリゴンとプレイヤーが衝突していなければ次のカウントへ
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
					poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
					continue;

				// ここに来たら衝突したということなのでフラグを立てる
				hitFlag = true;

				// 壁に当たったら壁に遮られない移動成分分だけ移動
				VECTOR slideVec; // スライドさせるベクトル

				// 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出
				slideVec = VCross(moveVec, poly->Normal);

				// 算出したベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出
				// これが元の移動成分から壁方向の移動成分を抜いたベクトル
				slideVec = VCross(poly->Normal, slideVec);

				// 座標を設定
				nowPos = VAdd(oldPos, slideVec);

				// 移動後の座標で壁ポリゴンと当たっていないかどうかを判定する
				for (j = 0; j < wallNum; ++j) {

					// j番目の壁ポリゴンのアドレスを取得
					poly = wall[j];

					// 当たっていたらループから抜ける
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
						poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
						break;
				}

				// j が wallNum の場合はどのポリゴンとも当たらなかったということなので
				// ループから抜ける
				if (j == wallNum) {
					hitFlag = 0;
					break;
				}
			}
		}
		else
		{
			// 移動していない場合の処理

			// 壁ポリゴンの数繰り返し
			for (i = 0; i < wallNum; ++i) {

				// i番目の壁ポリゴンのアドレスを取得
				poly = wall[i];

				// 当たっていたらループから抜ける
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
				{
					hitFlag = true;
					break;
				}
			}
		}

		// 壁に当たっていたら壁から押し出す処理を行う
		if (hitFlag == true)
		{
			// 壁からの押し出し処理を試みる最大数だけ繰り返し
			for (k = 0; k < PLAYER_HIT_TRYNUM; k++)
			{
				// 壁ポリゴンの数だけ繰り返し
				for (i = 0; i < wallNum; i++)
				{
					// i番目の壁ポリゴンのアドレスを取得
					poly = wall[i];

					// プレイヤーと当たっているかを判定
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
						poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
						continue;

					// 当たっていたら壁の法線方向に移動
					nowPos = VAdd(nowPos, VScale(poly->Normal, PLAYER_HIT_SLIDE_LENGTH));

					// 移動した上で壁ポリゴンと接触しているか判定
					for (j = 0; j < wallNum; j++)
					{
						// 当たっていたらループを抜ける
						poly = wall[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
							poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
							break;
					}

					// 全てのポリゴンと当たっていなかったらループ終了
					if (j == wallNum) break;
				}

				// i が wallNum ではない場合は全部のポリゴンで押し出しを試みる前に全ての壁ポリゴンと接触しなくなったということなのでループから抜ける
				if (i != wallNum) break;
			}
		}
	}

	// 床ポリゴンとの当たり判定
	if (floorNum != 0)
	{
		// ジャンプ中且つ上昇中の場合は処理を分岐
		if (state == 2 && speed_y > 0.0f)
		{
			float MinY;

			// 天井に頭をぶつける処理を行う

			// 一番低い天井にぶつける為の判定用変数を初期化
			MinY = 0.0f;

			// 当たったかどうかのフラグを折る
			hitFlag = false;

			// 床ポリゴンの数だけ繰り返し
			for (i = 0; i < floorNum; i++)
			{
				// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				poly = floor[i];

				// 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定
				lineRes = HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);

				// 接触していなかったら何もしない
				if (lineRes.HitFlag == FALSE) continue;

				// 既にポリゴンに当たっていて、且つ今まで検出した天井ポリゴンより高い場合は何もしない
				if (hitFlag == true && MinY < lineRes.Position.y) continue;

				// ポリゴンに当たったフラグを立てる
				hitFlag = true;

				// 接触したＹ座標を保存する
				MinY = lineRes.Position.y;
			}

			// 接触したポリゴンがあったかどうかで処理を分岐
			if (hitFlag == true)
			{
				// 接触した場合はプレイヤーのＹ座標を接触座標を元に更新
				nowPos.y = MinY - PLAYER_HIT_HEIGHT;

				// Ｙ軸方向の速度は反転
				speed_y = -speed_y;
			}
		}
		else
		{
			float MaxY;

			// 下降中かジャンプ中ではない場合の処理

			// 床ポリゴンに当たったかどうかのフラグを倒しておく
			hitFlag = false;

			// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
			MaxY = 0.0f;

			// 床ポリゴンの数だけ繰り返し
			for (i = 0; i < floorNum; i++)
			{
				// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				poly = floor[i];

				// ジャンプ中かどうかで処理を分岐
				if (state == 2)
				{
					// ジャンプ中の場合は頭の先から足先より少し低い位置の間で当たっているかを判定
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -PLAYER_HIT_HEIGHT, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				else
				{
					// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -PLAYER_HIT_HEIGHT, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}

				// 当たっていなかったら何もしない
				if (lineRes.HitFlag == FALSE) continue;

				// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
				if (hitFlag == true && MaxY > lineRes.Position.y) continue;

				// ポリゴンに当たったフラグを立てる
				hitFlag = true;

				// 接触したＹ座標を保存する
				MaxY = lineRes.Position.y;
			}

			// 床ポリゴンに当たったかどうかで処理を分岐
			if (hitFlag == true)
			{
				// 当たった場合

				// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
				nowPos.y = MaxY + PLAYER_HIT_HEIGHT;

				// Ｙ軸方向の移動速度は０に
				speed_y = 0.0f;

				// もしジャンプ中だった場合は着地状態にする
				if (state == 2)
				{
					// 移動していたかどうかで着地後の状態と再生するアニメーションを分岐する
					if (moveFlag)
					{
						// 移動している場合は走り状態に
						state = run;
					}
					else
					{
						// 移動していない場合は立ち止り状態に
						state = neutral;
					}
				}
			}
			else
			{
				// 床コリジョンに当たっていなくて且つジャンプ状態ではなかった場合は
				if (state != 2)
				{
					// ジャンプ中にする
					state = jump;
				}
			}
		}
	}

	DrawFormatString(0, 0, GetColor(255, 255, 255), "x = %.3f", position.x);
	DrawFormatString(0, 15, GetColor(255, 255, 255), "y = %.3f", position.y);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "z = %.3f", position.z);
	DrawFormatString(0, 45, GetColor(255, 255, 255), "ModelHandle = %d", modelHandle);

	// 新しい座標を保存する
	position = nowPos;

	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(modelHandle, position);

	// モデルを描画
	MV1DrawModel(modelHandle);

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	for (i = 0; i < hitDimNum; i++)
	{
		MV1CollResultPolyDimTerminate(hitDim[i]);
	}
}