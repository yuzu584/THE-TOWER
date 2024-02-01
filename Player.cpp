#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "Stage.h"
#include <math.h>

Player* player;

// プレイヤーの初期化
void Player::Initialize() {

	// 座標を設定
	position = VGet(1.0f, 1.0f, 1.0f);

	// 向く方向を設定
	targetDirection = VGet(1.0f, 0.0f, 0.0f);

	// 向きを設定
	angle = 0.0f;

	// y軸の速度を設定
	speed_y = 0.0f;

	// モデルの読み込み
	modelHandle = MV1LoadModel("BlockModel/WoodBox.mv1");

	// モデルのスケールを設定
	MV1SetScale(modelHandle, PLAYER_SCALE);
}

// プレイヤーの処理
void Player::Process() {

	// ルートフレームのＺ軸方向の移動パラメータを無効にする
	{
		MATRIX localMatrix;

		// ユーザー行列を解除する
		MV1ResetFrameUserLocalMatrix(modelHandle, 2);

		// 現在のルートフレームの行列を取得する
		localMatrix = MV1GetFrameLocalMatrix(modelHandle, 2);

		// Ｚ軸方向の平行移動成分を無効にする
		localMatrix.m[3][2] = 0.0f;

		// ユーザー行列として平行移動成分を無効にした行列をルートフレームにセットする
		MV1SetFrameUserLocalMatrix(modelHandle, 2, localMatrix);
	}

	// プレイヤーの移動方向のベクトルを算出
	{
		// 方向ボタン「↑」を押したときのプレイヤーの移動ベクトルはカメラの視線方向からＹ成分を抜いたもの
		moveVector_V = VSub(camera.GetTarget(), camera.GetPos());
		moveVector_V.y = 0.0f;

		// 方向ボタン「←」を押したときのプレイヤーの移動ベクトルは上を押したときの方向ベクトルとＹ軸のプラス方向のベクトルに垂直な方向
		moveVector_H = VCross(moveVector_V, VGet(0.0f, 1.0f, 0.0f));

		// 二つのベクトルを正規化( ベクトルの長さを１．０にすること )
		moveVector_V = VNorm(moveVector_V);
		moveVector_H = VNorm(moveVector_H);
	}

	// このフレームでの移動ベクトルを初期化
	moveVector = VGet(0.0f, 0.0f, 0.0f);

	// 移動したかどうかのフラグを初期状態では「移動していない」を表すfalseにする
	moveFlag = false;

	// 方向ボタン「←」が入力されたらカメラの見ている方向から見て左方向に移動する
	if (input.GetNowInput() & PAD_INPUT_4)
	{
		// 移動ベクトルに「←」が入力された時の移動ベクトルを加算する
		moveVector = VAdd(moveVector, moveVector_H);

		// 移動したかどうかのフラグを「移動した」にする
		moveFlag = true;
	}
	else
	{
		// 方向ボタン「→」が入力されたらカメラの見ている方向から見て右方向に移動する
		if (input.GetNowInput() & PAD_INPUT_6)
		{
			// 移動ベクトルに「←」が入力された時の移動ベクトルを反転したものを加算する
			moveVector = VAdd(moveVector, VScale(moveVector_H, -1.0f));

			// 移動したかどうかのフラグを「移動した」にする
			moveFlag = true;
		}
	}

	// 方向ボタン「↑」が入力されたらカメラの見ている方向に移動する
	if (input.GetNowInput() & PAD_INPUT_8)
	{
		// 移動ベクトルに「↑」が入力された時の移動ベクトルを加算する
		moveVector = VAdd(moveVector, moveVector_V);

		// 移動したかどうかのフラグを「移動した」にする
		moveFlag = true;
	}
	else
	{
		// 方向ボタン「↓」が入力されたらカメラの方向に移動する
		if (input.GetNowInput() & PAD_INPUT_5)
		{
			// 移動ベクトルに「↑」が入力された時の移動ベクトルを反転したものを加算する
			moveVector = VAdd(moveVector, VScale(moveVector_V, -1.0f));

			// 移動したかどうかのフラグを「移動した」にする
			moveFlag = true;
		}
	}

	// プレイヤーの状態が「ジャンプ」ではなく、且つボタン１が押されていたらジャンプする
	if (state != AnimeState::Jump && (input.GetEdgeInput() & PAD_INPUT_A))
	{
		// 状態を「ジャンプ」にする
		state = AnimeState::Jump;

		// Ｙ軸方向の速度をセット
		speed_y = PLAYER_JUMP_POWER;

		// ジャンプアニメーションの再生
		//Player_PlayAnim(state);
	}

	// 移動ボタンが押されたかどうかで処理を分岐
	if (moveFlag)
	{
		// 移動ベクトルを正規化したものをプレイヤーが向くべき方向として保存
		targetDirection = VNorm(moveVector);

		// プレイヤーが向くべき方向ベクトルをプレイヤーのスピード倍したものを移動ベクトルとする
		moveVector = VScale(targetDirection, PLAYER_MOVE_SPEED);

		// もし今まで「立ち止まり」状態だったら
		if (state == AnimeState::Neutral)
		{
			// 状態を「走り」にする
			state = AnimeState::Run;

			// 走りアニメーションを再生する
			//Player_PlayAnim(state);
		}
	}
	else
	{
		// このフレームで移動していなくて、且つ状態が「走り」だったら
		if (state == AnimeState::Run)
		{
			// 状態を「立ち止り」にする
			state = AnimeState::Neutral;

			// 立ち止りアニメーションを再生する
			//Player_PlayAnim(state);
		}
	}

	// 状態が「ジャンプ」の場合は
	if (state == AnimeState::Jump)
	{
		// Ｙ軸方向の速度を重力分減算する
		speed_y -= PLAYER_GRAVITY;

		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		moveVector.y = speed_y;
	}

	// プレイヤーの移動方向にモデルの方向を近づける
	AngleProcess();

	// 移動ベクトルを元にコリジョンを考慮しつつプレイヤーを移動
	Move(moveVector);

	// アニメーション処理
	//AnimProcess();
}

// プレイヤーの移動処理
void Player::Move(VECTOR moveVector) {
	bool moveFlag;									// 水平方向に移動したかどうかのフラグ( false:移動していない  true:移動した )
	bool hitFlag;									// ポリゴンに当たったかどうかを記憶しておくのに使う変数( false:当たっていない  true:当たった )
	MV1_COLL_RESULT_POLY_DIM hitDim[PLAYER_CHECK_BLOCK];// プレイヤーの周囲にあるポリゴンを検出した結果が代入される当たり判定結果構造体
	int kabeNum;									// 壁ポリゴンと判断されたポリゴンの数
	int yukaNum;									// 床ポリゴンと判断されたポリゴンの数
	MV1_COLL_RESULT_POLY* kabe[PLAYER_MAX_HITCOLL];	// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* yuka[PLAYER_MAX_HITCOLL];	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* poly;						// ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
	HITRESULT_LINE lineRes;							// 線分とポリゴンとの当たり判定の結果を代入する構造体
	VECTOR oldPos;									// 移動前の座標	
	VECTOR nowPos;									// 移動後の座標

	// 移動前の座標を保存
	oldPos = position;

	// 移動後の座標を算出
	nowPos = VAdd(position, moveVector);

	//---------------------------------------------------------
	// プレイヤーの周囲にあるステージポリゴンを取得する
	// ( 検出する範囲は移動距離も考慮する )

	//
	VECTOR checkPos[PLAYER_CHECK_BLOCK] = {
		{position},
		{VAdd(position, VGet(0, -1, 0))},
		{VAdd(position, VGet(0, 1, 0))},
		{VAdd(position, VGet(0, 0, -1))},
		{VAdd(position, VGet(0, 0, 1))},
		{VAdd(position, VGet(-1, 0, 0))},
		{VAdd(position, VGet(1, 0, 0))},
	};

	for (int i = 0; i < PLAYER_CHECK_BLOCK; ++i) {

		if (stage.CheckPos(position)) {
			hitDim[i] = MV1CollCheck_Sphere(
				stage.GetBlockPlacement(checkPos[i]).GetModelHandle(),
				-1,
				position,
				PLAYER_ENUM_DEFAULT_SIZE + VSize(moveVector));
		}
		else hitDim[i].Dim = NULL;
	}
	//---------------------------------------------------------

	// x軸かy軸方向に 0.01f 以上移動した場合は「移動した」フラグを１にする
	if (fabs(moveVector.x) > 0.01f || fabs(moveVector.z) > 0.01f)
	{
		moveFlag = true;
	}
	else
	{
		moveFlag = false;
	}

	// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断する
	{
		// 壁ポリゴンと床ポリゴンの数を初期化する
		kabeNum = 0;
		yukaNum = 0;

		for (int i = 0; i < PLAYER_CHECK_BLOCK; ++i) {
			// 検出されたポリゴンの数だけ繰り返し
			for (int j = 0; j < hitDim[i].HitNum; j++)
			{
				if (hitDim[i].Dim != NULL) {

					// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
					if (hitDim[i].Dim[j].Normal.y < 0.000001f && hitDim[i].Dim[j].Normal.y > -0.000001f)
					{
						// 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
						if (hitDim[i].Dim[j].Position[0].y > position.y + 1.0f ||
							hitDim[i].Dim[j].Position[1].y > position.y + 1.0f ||
							hitDim[i].Dim[j].Position[2].y > position.y + 1.0f)
						{
							// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
							if (kabeNum < PLAYER_MAX_HITCOLL)
							{
								// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
								kabe[kabeNum] = &hitDim[i].Dim[j];

								// 壁ポリゴンの数を加算する
								kabeNum++;
							}
						}
					}
					else
					{
						// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
						if (yukaNum < PLAYER_MAX_HITCOLL)
						{
							// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
							yuka[yukaNum] = &hitDim[i].Dim[j];

							// 床ポリゴンの数を加算する
							yukaNum++;
						}
					}
				}
			}
		}
	}

	// 壁ポリゴンとの当たり判定処理
	if (kabeNum != 0)
	{
		// 壁に当たったかどうかのフラグは初期状態では「当たっていない」にしておく
		hitFlag = false;

		// 移動したかどうかで処理を分岐
		if (moveFlag == true)
		{
			// 壁ポリゴンの数だけ繰り返し
			for (int i = 0; i < kabeNum; i++)
			{
				// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				poly = kabe[i];

				// ポリゴンとプレイヤーが当たっていなかったら次のカウントへ
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
					poly->Position[0], poly->Position[1], poly->Position[2]) == false)
				{
					continue;
				}

				// ここにきたらポリゴンとプレイヤーが当たっているということなので、ポリゴンに当たったフラグを立てる
				hitFlag = true;

				// 壁に当たったら壁に遮られない移動成分分だけ移動する
				{
					VECTOR slideVec;	// プレイヤーをスライドさせるベクトル

					//---------------------------------------------------------
					// 進行ベクトルと、壁の法線ベクトルの逆ベクトルを正規化したものの内積を求めたものを係数aとする
					float a = -VDot(moveVector, poly->Normal);

					// 進行ベクトルと、係数aを法線に掛けたベクトルを足したものが壁ずりベクトル
					slideVec = VAdd(moveVector, VGet(poly->Normal.x * a, poly->Normal.y * a, poly->Normal.z * a));

					// それを移動前の座標に足したものを新たな座標とする
					nowPos = VAdd(oldPos, slideVec);
					//---------------------------------------------------------
				}

				int j;

				// 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
				for (j = 0; j < kabeNum; j++)
				{
					// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					poly = kabe[j];

					// 当たっていたらループから抜ける
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == 1)
					{
						break;
					}
				}

				// j が kabeNum だった場合はどのポリゴンとも当たらなかったということなので
				// 壁に当たったフラグを倒した上でループから抜ける
				if (j == kabeNum)
				{
					hitFlag = false;
					break;
				}
			}
		}
		else
		{
			// 移動していない場合の処理

			// 壁ポリゴンの数だけ繰り返し
			for (int i = 0; i < kabeNum; i++)
			{
				// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				poly = kabe[i];

				// ポリゴンに当たっていたら当たったフラグを立てた上でループから抜ける
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == 1)
				{
					hitFlag = true;
					break;
				}
			}
		}

		// 壁に当たっていたら壁から押し出す処理を行う
		if (hitFlag)
		{
			// 壁からの押し出し処理を試みる最大数だけ繰り返し
			for (int k = 0; k < PLAYER_HIT_TRYNUM; k++)
			{
				int i;
				// 壁ポリゴンの数だけ繰り返し
				for (i = 0; i < kabeNum; i++)
				{
					// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					poly = kabe[i];

					// プレイヤーと当たっているかを判定
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == false)
					{
						continue;
					}

					//---------------------------------------------------------
					// 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
					nowPos = VAdd(nowPos, VScale(poly->Normal, PLAYER_HIT_SLIDE_LENGTH));
					//---------------------------------------------------------

					int j;

					// 移動した上で壁ポリゴンと接触しているかどうかを判定
					for (j = 0; j < kabeNum; j++)
					{
						// 当たっていたらループを抜ける
						poly = kabe[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == 1)
						{
							break;
						}
					}

					// 全てのポリゴンと当たっていなかったらここでループ終了
					if (j == kabeNum)
					{
						break;
					}
				}

				// i が kabeNum ではない場合は全部のポリゴンで押し出しを試みる前に全ての壁ポリゴンと接触しなくなったということなのでループから抜ける
				if (i != kabeNum)
				{
					break;
				}
			}
		}
	}

	// 床ポリゴンとの当たり判定
	if (yukaNum != 0)
	{
		// ジャンプ中且つ上昇中の場合は処理を分岐
		if (state == AnimeState::Jump && speed_y > 0.0f)
		{
			float MinY;

			// 天井に頭をぶつける処理を行う

			// 一番低い天井にぶつける為の判定用変数を初期化
			MinY = 0.0f;

			// 当たったかどうかのフラグを当たっていないを意味する０にしておく
			hitFlag = false;

			// 床ポリゴンの数だけ繰り返し
			for (int i = 0; i < yukaNum; i++)
			{
				// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				poly = yuka[i];

				//---------------------------------------------------------
				// 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定
				lineRes = HitCheck_Line_Triangle(
					nowPos,
					VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)),
					poly->Position[0],
					poly->Position[1],
					poly->Position[2]);
				//---------------------------------------------------------

				// 接触していなかったら何もしない
				if (lineRes.HitFlag == false)
				{
					continue;
				}

				// 既にポリゴンに当たっていて、且つ今まで検出した天井ポリゴンより高い場合は何もしない
				if (hitFlag == true && MinY < lineRes.Position.y)
				{
					continue;
				}

				// ポリゴンに当たったフラグを立てる
				hitFlag = true;

				// 接触したＹ座標を保存する
				MinY = lineRes.Position.y;
			}

			// 接触したポリゴンがあったかどうかで処理を分岐
			if (hitFlag)
			{
				// 接触した場合はプレイヤーのＹ座標を接触座標を元に更新
				nowPos.y = MinY - PLAYER_HIT_HEIGHT;

				// Ｙ軸方向の速度は反転
				speed_y = -speed_y;
			}
		}
		else
		{
			// 下降中かジャンプ中ではない場合の処理
			float MaxY;

			// 床ポリゴンに当たったかどうかのフラグを倒しておく
			hitFlag = false;

			// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
			MaxY = 0.0f;

			// 床ポリゴンの数だけ繰り返し
			for (int i = 0; i < yukaNum; i++)
			{
				// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				poly = yuka[i];

				// ジャンプ中かどうかで処理を分岐
				if (state == AnimeState::Jump)
				{
					// ジャンプ中の場合は頭の先から足先より少し低い位置の間で当たっているかを判定
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -0.1f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				else
				{
					// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -0.3f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}

				// 当たっていなかったら何もしない
				if (lineRes.HitFlag == false)
				{
					continue;
				}

				// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
				if (hitFlag == true && MaxY > lineRes.Position.y)
				{
					continue;
				}

				// ポリゴンに当たったフラグを立てる
				hitFlag = true;

				// 接触したＹ座標を保存する
				MaxY = lineRes.Position.y;
			}

			// 床ポリゴンに当たったかどうかで処理を分岐
			if (hitFlag)
			{
				// 当たった場合

				// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
				nowPos.y = MaxY;

				// Ｙ軸方向の移動速度は０に
				speed_y = 0.0f;

				// もしジャンプ中だった場合は着地状態にする
				if (state == AnimeState::Jump)
				{
					// 移動していたかどうかで着地後の状態と再生するアニメーションを分岐する
					if (moveFlag)
					{
						// 移動している場合は走り状態に
						state = AnimeState::Run;
					}
					else
					{
						// 移動していない場合は立ち止り状態に
						state = AnimeState::Neutral;
					}
					//Player_PlayAnim(state);

					// 着地時はアニメーションのブレンドは行わない
					//animBlendRate = 1.0f;
				}
			}
			else
			{
				// 床コリジョンに当たっていなくて且つジャンプ状態ではなかった場合は
				if (state != AnimeState::Jump)
				{
					// ジャンプ中にする
					state = AnimeState::Jump;

					// ちょっとだけジャンプする
					speed_y = PLAYER_FALL_UP__POWER;

					// アニメーションを再生
					//Player_PlayAnim(AnimeState::Jump);
				}
			}
		}
	}

	// 新しい座標を保存する
	position = nowPos;

	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(modelHandle, position);

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	for (int i = 0; i < PLAYER_CHECK_BLOCK; ++i) {
		MV1CollResultPolyDimTerminate(hitDim[i]);
	}

	// プレイヤーの座標を表示
	DrawFormatString(0, 0, GetColor(255, 255, 255), "x = %.3f", position.x);
	DrawFormatString(0, 15, GetColor(255, 255, 255), "y = %.3f", position.y);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "z = %.3f", position.z);
}

// プレイヤーの向きを変える処理
void Player::AngleProcess() {
	float targetAngle;			// 目標角度
	float saAngle;				// 目標角度と現在の角度との差

	// 目標の方向ベクトルから角度値を算出する
	targetAngle = atan2f(targetDirection.x, targetDirection.z);

	// 目標の角度と現在の角度との差を割り出す
	{
		// 最初は単純に引き算
		saAngle = targetAngle - angle;

		// ある方向からある方向の差が１８０度以上になることは無いので
		// 差の値が１８０度以上になっていたら修正する
		if (saAngle < -DX_PI_F)
		{
			saAngle += DX_TWO_PI_F;
		}
		else if (saAngle > DX_PI_F)
		{
			saAngle -= DX_TWO_PI_F;
		}
	}

	// 角度の差が０に近づける
	if (saAngle > 0.0f)
	{
		// 差がプラスの場合は引く
		saAngle -= PLAYER_ANGLE_SPEED;
		if (saAngle < 0.0f)
		{
			saAngle = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		saAngle += PLAYER_ANGLE_SPEED;
		if (saAngle > 0.0f)
		{
			saAngle = 0.0f;
		}
	}

	// モデルの角度を更新
	angle = targetAngle - saAngle;
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

// プレイヤーのアニメーション処理
void Player::AnimProcess() {

}

// コンストラクタ
Player::Player() {

	// プレイヤーの初期化
	Initialize();
}

// デストラクタ
Player::~Player() {

}