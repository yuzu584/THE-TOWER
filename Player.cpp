#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "Stage.h"
#include <math.h>

Player* player;

// �v���C���[�̏�����
void Player::Initialize() {

	// ���W��ݒ�
	position = VGet(1.0f, 1.0f, 1.0f);

	// ����������ݒ�
	targetDirection = VGet(1.0f, 0.0f, 0.0f);

	// ������ݒ�
	angle = 0.0f;

	// y���̑��x��ݒ�
	speed_y = 0.0f;

	// ���f���̓ǂݍ���
	modelHandle = MV1LoadModel("BlockModel/WoodBox.mv1");

	// ���f���̃X�P�[����ݒ�
	MV1SetScale(modelHandle, PLAYER_SCALE);
}

// �v���C���[�̏���
void Player::Process() {

	// ���[�g�t���[���̂y�������̈ړ��p�����[�^�𖳌��ɂ���
	{
		MATRIX localMatrix;

		// ���[�U�[�s�����������
		MV1ResetFrameUserLocalMatrix(modelHandle, 2);

		// ���݂̃��[�g�t���[���̍s����擾����
		localMatrix = MV1GetFrameLocalMatrix(modelHandle, 2);

		// �y�������̕��s�ړ������𖳌��ɂ���
		localMatrix.m[3][2] = 0.0f;

		// ���[�U�[�s��Ƃ��ĕ��s�ړ������𖳌��ɂ����s������[�g�t���[���ɃZ�b�g����
		MV1SetFrameUserLocalMatrix(modelHandle, 2, localMatrix);
	}

	// �v���C���[�̈ړ������̃x�N�g�����Z�o
	{
		// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���̓J�����̎�����������x�����𔲂�������
		moveVector_V = VSub(camera.GetTarget(), camera.GetPos());
		moveVector_V.y = 0.0f;

		// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���͏���������Ƃ��̕����x�N�g���Ƃx���̃v���X�����̃x�N�g���ɐ����ȕ���
		moveVector_H = VCross(moveVector_V, VGet(0.0f, 1.0f, 0.0f));

		// ��̃x�N�g���𐳋K��( �x�N�g���̒������P�D�O�ɂ��邱�� )
		moveVector_V = VNorm(moveVector_V);
		moveVector_H = VNorm(moveVector_H);
	}

	// ���̃t���[���ł̈ړ��x�N�g����������
	moveVector = VGet(0.0f, 0.0f, 0.0f);

	// �ړ��������ǂ����̃t���O��������Ԃł́u�ړ����Ă��Ȃ��v��\��false�ɂ���
	moveFlag = false;

	// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă���������猩�č������Ɉړ�����
	if (input.GetNowInput() & PAD_INPUT_4)
	{
		// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g�������Z����
		moveVector = VAdd(moveVector, moveVector_H);

		// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
		moveFlag = true;
	}
	else
	{
		// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă���������猩�ĉE�����Ɉړ�����
		if (input.GetNowInput() & PAD_INPUT_6)
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g���𔽓]�������̂����Z����
			moveVector = VAdd(moveVector, VScale(moveVector_H, -1.0f));

			// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
			moveFlag = true;
		}
	}

	// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă�������Ɉړ�����
	if (input.GetNowInput() & PAD_INPUT_8)
	{
		// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g�������Z����
		moveVector = VAdd(moveVector, moveVector_V);

		// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
		moveFlag = true;
	}
	else
	{
		// �����{�^���u���v�����͂��ꂽ��J�����̕����Ɉړ�����
		if (input.GetNowInput() & PAD_INPUT_5)
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g���𔽓]�������̂����Z����
			moveVector = VAdd(moveVector, VScale(moveVector_V, -1.0f));

			// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
			moveFlag = true;
		}
	}

	// �v���C���[�̏�Ԃ��u�W�����v�v�ł͂Ȃ��A���{�^���P��������Ă�����W�����v����
	if (state != AnimeState::Jump && (input.GetEdgeInput() & PAD_INPUT_A))
	{
		// ��Ԃ��u�W�����v�v�ɂ���
		state = AnimeState::Jump;

		// �x�������̑��x���Z�b�g
		speed_y = PLAYER_JUMP_POWER;

		// �W�����v�A�j���[�V�����̍Đ�
		//Player_PlayAnim(state);
	}

	// �ړ��{�^���������ꂽ���ǂ����ŏ����𕪊�
	if (moveFlag)
	{
		// �ړ��x�N�g���𐳋K���������̂��v���C���[�������ׂ������Ƃ��ĕۑ�
		targetDirection = VNorm(moveVector);

		// �v���C���[�������ׂ������x�N�g�����v���C���[�̃X�s�[�h�{�������̂��ړ��x�N�g���Ƃ���
		moveVector = VScale(targetDirection, PLAYER_MOVE_SPEED);

		// �������܂Łu�����~�܂�v��Ԃ�������
		if (state == AnimeState::Neutral)
		{
			// ��Ԃ��u����v�ɂ���
			state = AnimeState::Run;

			// ����A�j���[�V�������Đ�����
			//Player_PlayAnim(state);
		}
	}
	else
	{
		// ���̃t���[���ňړ����Ă��Ȃ��āA����Ԃ��u����v��������
		if (state == AnimeState::Run)
		{
			// ��Ԃ��u�����~��v�ɂ���
			state = AnimeState::Neutral;

			// �����~��A�j���[�V�������Đ�����
			//Player_PlayAnim(state);
		}
	}

	// ��Ԃ��u�W�����v�v�̏ꍇ��
	if (state == AnimeState::Jump)
	{
		// �x�������̑��x���d�͕����Z����
		speed_y -= PLAYER_GRAVITY;

		// �ړ��x�N�g���̂x�������x�������̑��x�ɂ���
		moveVector.y = speed_y;
	}

	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	AngleProcess();

	// �ړ��x�N�g�������ɃR���W�������l�����v���C���[���ړ�
	Move(moveVector);

	// �A�j���[�V��������
	//AnimProcess();
}

// �v���C���[�̈ړ�����
void Player::Move(VECTOR moveVector) {
	bool moveFlag;									// ���������Ɉړ��������ǂ����̃t���O( false:�ړ����Ă��Ȃ�  true:�ړ����� )
	bool hitFlag;									// �|���S���ɓ����������ǂ������L�����Ă����̂Ɏg���ϐ�( false:�������Ă��Ȃ�  true:�������� )
	MV1_COLL_RESULT_POLY_DIM hitDim[PLAYER_CHECK_BLOCK];// �v���C���[�̎��͂ɂ���|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	int kabeNum;									// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int yukaNum;									// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	MV1_COLL_RESULT_POLY* kabe[PLAYER_MAX_HITCOLL];	// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* yuka[PLAYER_MAX_HITCOLL];	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* poly;						// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
	HITRESULT_LINE lineRes;							// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
	VECTOR oldPos;									// �ړ��O�̍��W	
	VECTOR nowPos;									// �ړ���̍��W

	// �ړ��O�̍��W��ۑ�
	oldPos = position;

	// �ړ���̍��W���Z�o
	nowPos = VAdd(position, moveVector);

	//---------------------------------------------------------
	// �v���C���[�̎��͂ɂ���X�e�[�W�|���S�����擾����
	// ( ���o����͈͈͂ړ��������l������ )

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

	// x����y�������� 0.01f �ȏ�ړ������ꍇ�́u�ړ������v�t���O���P�ɂ���
	if (fabs(moveVector.x) > 0.01f || fabs(moveVector.z) > 0.01f)
	{
		moveFlag = true;
	}
	else
	{
		moveFlag = false;
	}

	// ���o���ꂽ�|���S�����ǃ|���S��( �w�y���ʂɐ����ȃ|���S�� )�����|���S��( �w�y���ʂɐ����ł͂Ȃ��|���S�� )���𔻒f����
	{
		// �ǃ|���S���Ə��|���S���̐�������������
		kabeNum = 0;
		yukaNum = 0;

		for (int i = 0; i < PLAYER_CHECK_BLOCK; ++i) {
			// ���o���ꂽ�|���S���̐������J��Ԃ�
			for (int j = 0; j < hitDim[i].HitNum; j++)
			{
				if (hitDim[i].Dim != NULL) {

					// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
					if (hitDim[i].Dim[j].Normal.y < 0.000001f && hitDim[i].Dim[j].Normal.y > -0.000001f)
					{
						// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
						if (hitDim[i].Dim[j].Position[0].y > position.y + 1.0f ||
							hitDim[i].Dim[j].Position[1].y > position.y + 1.0f ||
							hitDim[i].Dim[j].Position[2].y > position.y + 1.0f)
						{
							// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
							if (kabeNum < PLAYER_MAX_HITCOLL)
							{
								// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
								kabe[kabeNum] = &hitDim[i].Dim[j];

								// �ǃ|���S���̐������Z����
								kabeNum++;
							}
						}
					}
					else
					{
						// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
						if (yukaNum < PLAYER_MAX_HITCOLL)
						{
							// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
							yuka[yukaNum] = &hitDim[i].Dim[j];

							// ���|���S���̐������Z����
							yukaNum++;
						}
					}
				}
			}
		}
	}

	// �ǃ|���S���Ƃ̓����蔻�菈��
	if (kabeNum != 0)
	{
		// �ǂɓ����������ǂ����̃t���O�͏�����Ԃł́u�������Ă��Ȃ��v�ɂ��Ă���
		hitFlag = false;

		// �ړ��������ǂ����ŏ����𕪊�
		if (moveFlag == true)
		{
			// �ǃ|���S���̐������J��Ԃ�
			for (int i = 0; i < kabeNum; i++)
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				poly = kabe[i];

				// �|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
					poly->Position[0], poly->Position[1], poly->Position[2]) == false)
				{
					continue;
				}

				// �����ɂ�����|���S���ƃv���C���[���������Ă���Ƃ������ƂȂ̂ŁA�|���S���ɓ��������t���O�𗧂Ă�
				hitFlag = true;

				// �ǂɓ���������ǂɎՂ��Ȃ��ړ������������ړ�����
				{
					VECTOR slideVec;	// �v���C���[���X���C�h������x�N�g��

					//---------------------------------------------------------
					// �i�s�x�N�g���ƁA�ǂ̖@���x�N�g���̋t�x�N�g���𐳋K���������̂̓��ς����߂����̂��W��a�Ƃ���
					float a = -VDot(moveVector, poly->Normal);

					// �i�s�x�N�g���ƁA�W��a��@���Ɋ|�����x�N�g���𑫂������̂��ǂ���x�N�g��
					slideVec = VAdd(moveVector, VGet(poly->Normal.x * a, poly->Normal.y * a, poly->Normal.z * a));

					// ������ړ��O�̍��W�ɑ��������̂�V���ȍ��W�Ƃ���
					nowPos = VAdd(oldPos, slideVec);
					//---------------------------------------------------------
				}

				int j;

				// �V���Ȉړ����W�ŕǃ|���S���Ɠ������Ă��Ȃ����ǂ����𔻒肷��
				for (j = 0; j < kabeNum; j++)
				{
					// j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					poly = kabe[j];

					// �������Ă����烋�[�v���甲����
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == 1)
					{
						break;
					}
				}

				// j �� kabeNum �������ꍇ�͂ǂ̃|���S���Ƃ�������Ȃ������Ƃ������ƂȂ̂�
				// �ǂɓ��������t���O��|������Ń��[�v���甲����
				if (j == kabeNum)
				{
					hitFlag = false;
					break;
				}
			}
		}
		else
		{
			// �ړ����Ă��Ȃ��ꍇ�̏���

			// �ǃ|���S���̐������J��Ԃ�
			for (int i = 0; i < kabeNum; i++)
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				poly = kabe[i];

				// �|���S���ɓ������Ă����瓖�������t���O�𗧂Ă���Ń��[�v���甲����
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == 1)
				{
					hitFlag = true;
					break;
				}
			}
		}

		// �ǂɓ������Ă�����ǂ��牟���o���������s��
		if (hitFlag)
		{
			// �ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
			for (int k = 0; k < PLAYER_HIT_TRYNUM; k++)
			{
				int i;
				// �ǃ|���S���̐������J��Ԃ�
				for (i = 0; i < kabeNum; i++)
				{
					// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					poly = kabe[i];

					// �v���C���[�Ɠ������Ă��邩�𔻒�
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == false)
					{
						continue;
					}

					//---------------------------------------------------------
					// �������Ă�����K�苗�����v���C���[��ǂ̖@�������Ɉړ�������
					nowPos = VAdd(nowPos, VScale(poly->Normal, PLAYER_HIT_SLIDE_LENGTH));
					//---------------------------------------------------------

					int j;

					// �ړ�������ŕǃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
					for (j = 0; j < kabeNum; j++)
					{
						// �������Ă����烋�[�v�𔲂���
						poly = kabe[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == 1)
						{
							break;
						}
					}

					// �S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
					if (j == kabeNum)
					{
						break;
					}
				}

				// i �� kabeNum �ł͂Ȃ��ꍇ�͑S���̃|���S���ŉ����o�������݂�O�ɑS�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����Ƃ������ƂȂ̂Ń��[�v���甲����
				if (i != kabeNum)
				{
					break;
				}
			}
		}
	}

	// ���|���S���Ƃ̓����蔻��
	if (yukaNum != 0)
	{
		// �W�����v�����㏸���̏ꍇ�͏����𕪊�
		if (state == AnimeState::Jump && speed_y > 0.0f)
		{
			float MinY;

			// �V��ɓ����Ԃ��鏈�����s��

			// ��ԒႢ�V��ɂԂ���ׂ̔���p�ϐ���������
			MinY = 0.0f;

			// �����������ǂ����̃t���O�𓖂����Ă��Ȃ����Ӗ�����O�ɂ��Ă���
			hitFlag = false;

			// ���|���S���̐������J��Ԃ�
			for (int i = 0; i < yukaNum; i++)
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				poly = yuka[i];

				//---------------------------------------------------------
				// ���悩�瓪�̍����܂ł̊ԂŃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
				lineRes = HitCheck_Line_Triangle(
					nowPos,
					VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)),
					poly->Position[0],
					poly->Position[1],
					poly->Position[2]);
				//---------------------------------------------------------

				// �ڐG���Ă��Ȃ������牽�����Ȃ�
				if (lineRes.HitFlag == false)
				{
					continue;
				}

				// ���Ƀ|���S���ɓ������Ă��āA�����܂Ō��o�����V��|���S����荂���ꍇ�͉������Ȃ�
				if (hitFlag == true && MinY < lineRes.Position.y)
				{
					continue;
				}

				// �|���S���ɓ��������t���O�𗧂Ă�
				hitFlag = true;

				// �ڐG�����x���W��ۑ�����
				MinY = lineRes.Position.y;
			}

			// �ڐG�����|���S�������������ǂ����ŏ����𕪊�
			if (hitFlag)
			{
				// �ڐG�����ꍇ�̓v���C���[�̂x���W��ڐG���W�����ɍX�V
				nowPos.y = MinY - PLAYER_HIT_HEIGHT;

				// �x�������̑��x�͔��]
				speed_y = -speed_y;
			}
		}
		else
		{
			// ���~�����W�����v���ł͂Ȃ��ꍇ�̏���
			float MaxY;

			// ���|���S���ɓ����������ǂ����̃t���O��|���Ă���
			hitFlag = false;

			// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
			MaxY = 0.0f;

			// ���|���S���̐������J��Ԃ�
			for (int i = 0; i < yukaNum; i++)
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				poly = yuka[i];

				// �W�����v�����ǂ����ŏ����𕪊�
				if (state == AnimeState::Jump)
				{
					// �W�����v���̏ꍇ�͓��̐悩�瑫���菭���Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -0.1f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				else
				{
					// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -0.3f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}

				// �������Ă��Ȃ������牽�����Ȃ�
				if (lineRes.HitFlag == false)
				{
					continue;
				}

				// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
				if (hitFlag == true && MaxY > lineRes.Position.y)
				{
					continue;
				}

				// �|���S���ɓ��������t���O�𗧂Ă�
				hitFlag = true;

				// �ڐG�����x���W��ۑ�����
				MaxY = lineRes.Position.y;
			}

			// ���|���S���ɓ����������ǂ����ŏ����𕪊�
			if (hitFlag)
			{
				// ���������ꍇ

				// �ڐG�����|���S���ň�ԍ����x���W���v���C���[�̂x���W�ɂ���
				nowPos.y = MaxY;

				// �x�������̈ړ����x�͂O��
				speed_y = 0.0f;

				// �����W�����v���������ꍇ�͒��n��Ԃɂ���
				if (state == AnimeState::Jump)
				{
					// �ړ����Ă������ǂ����Œ��n��̏�ԂƍĐ�����A�j���[�V�����𕪊򂷂�
					if (moveFlag)
					{
						// �ړ����Ă���ꍇ�͑����Ԃ�
						state = AnimeState::Run;
					}
					else
					{
						// �ړ����Ă��Ȃ��ꍇ�͗����~���Ԃ�
						state = AnimeState::Neutral;
					}
					//Player_PlayAnim(state);

					// ���n���̓A�j���[�V�����̃u�����h�͍s��Ȃ�
					//animBlendRate = 1.0f;
				}
			}
			else
			{
				// ���R���W�����ɓ������Ă��Ȃ��Ċ��W�����v��Ԃł͂Ȃ������ꍇ��
				if (state != AnimeState::Jump)
				{
					// �W�����v���ɂ���
					state = AnimeState::Jump;

					// ������Ƃ����W�����v����
					speed_y = PLAYER_FALL_UP__POWER;

					// �A�j���[�V�������Đ�
					//Player_PlayAnim(AnimeState::Jump);
				}
			}
		}
	}

	// �V�������W��ۑ�����
	position = nowPos;

	// �v���C���[�̃��f���̍��W���X�V����
	MV1SetPosition(modelHandle, position);

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	for (int i = 0; i < PLAYER_CHECK_BLOCK; ++i) {
		MV1CollResultPolyDimTerminate(hitDim[i]);
	}

	// �v���C���[�̍��W��\��
	DrawFormatString(0, 0, GetColor(255, 255, 255), "x = %.3f", position.x);
	DrawFormatString(0, 15, GetColor(255, 255, 255), "y = %.3f", position.y);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "z = %.3f", position.z);
}

// �v���C���[�̌�����ς��鏈��
void Player::AngleProcess() {
	float targetAngle;			// �ڕW�p�x
	float saAngle;				// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

	// �ڕW�̕����x�N�g������p�x�l���Z�o����
	targetAngle = atan2f(targetDirection.x, targetDirection.z);

	// �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
	{
		// �ŏ��͒P���Ɉ����Z
		saAngle = targetAngle - angle;

		// ����������炠������̍����P�W�O�x�ȏ�ɂȂ邱�Ƃ͖����̂�
		// ���̒l���P�W�O�x�ȏ�ɂȂ��Ă�����C������
		if (saAngle < -DX_PI_F)
		{
			saAngle += DX_TWO_PI_F;
		}
		else if (saAngle > DX_PI_F)
		{
			saAngle -= DX_TWO_PI_F;
		}
	}

	// �p�x�̍����O�ɋ߂Â���
	if (saAngle > 0.0f)
	{
		// �����v���X�̏ꍇ�͈���
		saAngle -= PLAYER_ANGLE_SPEED;
		if (saAngle < 0.0f)
		{
			saAngle = 0.0f;
		}
	}
	else
	{
		// �����}�C�i�X�̏ꍇ�͑���
		saAngle += PLAYER_ANGLE_SPEED;
		if (saAngle > 0.0f)
		{
			saAngle = 0.0f;
		}
	}

	// ���f���̊p�x���X�V
	angle = targetAngle - saAngle;
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

// �v���C���[�̃A�j���[�V��������
void Player::AnimProcess() {

}

// �R���X�g���N�^
Player::Player() {

	// �v���C���[�̏�����
	Initialize();
}

// �f�X�g���N�^
Player::~Player() {

}