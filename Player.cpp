#include "Player.h"
#include "Camera.h"
#include "Input.h"

PLAYER player;

// ����������
void PLAYER::Initialize() {

	// ���W��ݒ�
	position = VGet(4.5f, 1.0f, 4.5f);

	// ���f���ǂݍ���
	modelHandle = MV1LoadModel("BlockModel/WoodBox.mv1");

	// �X�P�[����ݒ�
	MV1SetScale(modelHandle, VGet(0.5f, 0.5f, 0.5f));
}

// �v���C���[�̏���
void PLAYER::Process() {
	VECTOR upMoveVec;   // ����͎��̈ړ�����
	VECTOR leftMoveVec; // �����͎��̈ړ�����
	VECTOR moveVec;     // �ŏI�I�Ȉړ�����
	int moveFlag;       // �ړ��������ǂ���( true : �ړ�����  false : �ړ����Ă��Ȃ� )

	// �ړ������x�N�g�����Z�o
	// ������̈ړ��x�N�g���̓J�����̎�����������Y�����𔲂�������
	upMoveVec = VSub(camera.GetTarget(), camera.GetPos());
	upMoveVec.y = 0.0f;

	// �������̈ړ��x�N�g���͏�����x�N�g����Y���̃v���X�����̃x�N�g���ɐ����ȕ���
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// ��̃x�N�g���𐳋K��
	upMoveVec = VNorm(upMoveVec);
	leftMoveVec = VNorm(leftMoveVec);

	// ���̃t���[���̈ړ��x�N�g����������
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	// �ړ����Ă��Ȃ���Ԃɂ���
	moveFlag = false;

	// �����͂��ꂽ��J�������猩�č������Ɉړ�
	if (input.GetNowInput() & PAD_INPUT_4)
	{
		// �ړ��x�N�g���ɍ����͎��̈ړ��x�N�g�������Z
		moveVec = VAdd(moveVec, leftMoveVec);

		// �ړ��t���O�𗧂Ă�
		moveFlag = 1;
	}
	else
		// �E���͂��ꂽ��J�������猩�ĉE�����Ɉړ�
		if (input.GetNowInput() & PAD_INPUT_6)
		{
			// �ړ��x�N�g���ɍ����͎��̈ړ��x�N�g���𔽓]�������̂����Z
			moveVec = VAdd(moveVec, VScale(leftMoveVec, -1.0f));

			// �ړ��t���O�𗧂Ă�
			moveFlag = 1;
		}

	// ����͂��ꂽ��J�����̌��Ă�������Ɉړ�
	if (input.GetNowInput() & PAD_INPUT_8)
	{
		// �ړ��x�N�g���ɏ���͎��̈ړ��x�N�g�������Z
		moveVec = VAdd(moveVec, upMoveVec);

		// �ړ��t���O�𗧂Ă�
		moveFlag = 1;
	}
	else
		// �����͂��ꂽ��J�����̕����Ɉړ�����
		if (input.GetNowInput() & PAD_INPUT_5)
		{
			// ����͎��̈ړ��x�N�g���𔽓]�������̂����Z
			moveVec = VAdd(moveVec, VScale(upMoveVec, -1.0f));

			// �ړ��t���O�𗧂Ă�
			moveFlag = 1;
		}

	// �v���C���[�̏�Ԃ��u�W�����v�v�ł͂Ȃ��A���W�����v�{�^����������Ă�����W�����v
	if (state != 2 && (input.GetEdgeInput() & PAD_INPUT_A))
	{
		// ��Ԃ��u�W�����v�v�ɂ���
		state = jump;

		// Y�������̑��x���Z�b�g
		speed_y = PLAYER_JUMP_POWER;
	}

	// �ړ����Ă��邩�ǂ����ŕ���
	if (moveFlag)
	{
		// �ړ��x�N�g���𐳋K���������̂��v���C���[�����������Ƃ��Đݒ�
		targetDirection = VNorm(moveVec);

		// �v���C���[�������ׂ������x�N�g�����v���C���[�̃X�s�[�h�{�������̂��ړ��x�N�g���Ƃ���
		moveVec = VScale(targetDirection, PLAYER_MOVE_SPEED);

		// �u�����v��ԂȂ�
		if (state == 0)
		{
			// ��Ԃ��u����v�ɂ���
			state = run;
		}
	}
	else
	{
		// ���̃t���[���ňړ����Ă��Ȃ��A����Ԃ��u����v�Ȃ�
		if (state == 1)
		{
			// ��Ԃ��u�����v�ɂ���
			state = neutral;
		}
	}

	// Y�������̑��x���d�͕����Z����
	speed_y -= PLAYER_GRAVITY;

	// �ړ��x�N�g����Y������Y�������̑��x�ɂ���
	moveVec.y = speed_y;

	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	AngleProcess();

	// �R���W�������l�����v���C���[���ړ�
	Move(moveVec);
}

// �����̏���
void PLAYER::AngleProcess() {
	float targetAngle;	// �ڕW�p�x
	float angleDiff;	// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

	// �ڕW�̕����x�N�g������p�x���Z�o
	targetAngle = atan2(targetDirection.x, targetDirection.z);

	// �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
	{
		// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍����v�Z
		angleDiff = targetAngle - angle;

		// ����180�x�ȏ�ɂȂ��Ă�����C��
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

	// �p�x�̍���0�ɋ߂Â���
	if (angleDiff > 0.0f)
	{
		// �����v���X�̏ꍇ�͈���
		angleDiff -= PLAYER_ANGLE_SPEED;
		if (angleDiff < 0.0f)
		{
			angleDiff = 0.0f;
		}
	}
	else
	{
		// �����}�C�i�X�̏ꍇ�͑���
		angleDiff += PLAYER_ANGLE_SPEED;
		if (angleDiff > 0.0f)
		{
			angleDiff = 0.0f;
		}
	}

	// ���f���̊p�x���X�V
	angle = targetAngle - angleDiff;
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

// �ړ�����
void PLAYER::Move(VECTOR moveVec) {
	int i, j, k;										// �ėp�J�E���^�ϐ�
	int moveFlag;										// ���������Ɉړ��������ǂ����̃t���O( 0:�ړ����Ă��Ȃ�  1:�ړ����� )
	int hitFlag;										// �|���S���ɓ����������ǂ������L������ϐ�( 0:�������Ă��Ȃ�  1:�������� )
	MV1_COLL_RESULT_POLY_DIM hitDim[7];					// �v���C���[�̎��͂̃|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	int hitDimNum;										// HitDim �̗L���Ȕz��v�f��
	int wallNum;										// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int floorNum;										// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	MV1_COLL_RESULT_POLY* wall[PLAYER_MAX_HITCOLL];		// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* floor[PLAYER_MAX_HITCOLL];	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* poly;							// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^
	HITRESULT_LINE lineRes;								// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
	VECTOR oldPos;										// �ړ��O�̍��W	
	VECTOR nowPos;										// �ړ���̍��W

	// �ړ��O�̍��W��ۑ�
	oldPos = position;

	// �ړ���̍��W���Z�o
	nowPos = VAdd(position, moveVec);

	// �擾����u���b�N�̍��W
	VECTOR blockPos[7] = {
		{nowPos},									// �v���C���[�̍��W
		{VAdd(nowPos, VGet(1.0f, 0.0f, 0.0f))},		// X���Ƀv���X�̃u���b�N�̍��W
		{VAdd(nowPos, VGet(0.0f, 0.0f, -1.0f))},	// Z���Ƀ}�C�i�X�̃u���b�N�̍��W
		{VAdd(nowPos, VGet(-1.0f, 0.0f, 0.0f))},	// X���Ƀ}�C�i�X�̃u���b�N�̍��W
		{VAdd(nowPos, VGet(0.0f, 0.0f, 1.0f))},		// Z���Ƀv���X�̃u���b�N�̍��W
		{VAdd(nowPos, VGet(0.0f, 1.0f, 0.0f))},		// Y���Ƀv���X�̃u���b�N�̍��W
		{VAdd(nowPos, VGet(0.0f, -1.0f, 0.0f))},	// Y���Ƀ}�C�i�X�̃u���b�N�̍��W
	};

	// �v���C���[�̎��͂̃u���b�N�̃|���S�����擾
	for ( i = 0; i < 7; ++i)
	{
		if (stage.CheckPos(blockPos[i])) {
			hitDim[i] = MV1CollCheck_Sphere(stage.GetBlockPlacement(blockPos[i]).GetModelHandle(), -1, position, PLAYER_COLL_SPHERE_SIZE + VSize(moveVec));
		}
		else hitDim[i].Dim = NULL;
	}

	// hitDim�̗L���Ȑ��͌��o�����u���b�N�̐�
	hitDimNum = 7;

	// x����z���� 0.01f �ȏ�ړ������ꍇ�͈ړ��t���O�𗧂Ă�
	if (fabs(moveVec.x) > 0.01f || fabs(moveVec.z) > 0.01f)
	{
		moveFlag = true;
	}
	else
	{
		moveFlag = false;
	}

	// ���o�����|���S�����ǃ|���S�������|���S�����𔻒f
	// �ϐ���������
	wallNum = 0;
	floorNum = 0;

	// ���o�����u���b�N�̃|���S���̐��J��Ԃ�
	for (j = 0; j < hitDimNum; ++j) {
		for (i = 0; i < hitDim[j].HitNum; ++i) {

			if (hitDim[i].Dim != NULL) {

				// �ǃ|���S�������f(�|���S���̖@����Y����������Ȃ�0�ɋ߂����Ŕ��f)
				if (hitDim[j].Dim[i].Normal.y < 0.000001f && hitDim[j].Dim[i].Normal.y > -0.000001f) {

					// �ǃ|���S���̏ꍇ�ł��A�v���C���[�̑�����Y���W+0.1f��荂���|���S���̂ݓ����蔻����s��
					if (hitDim[j].Dim[i].Position[0].y > position.y + 0.1f ||
						hitDim[j].Dim[i].Position[1].y > position.y + 0.1f ||
						hitDim[j].Dim[i].Position[2].y > position.y + 0.1f) {

						// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ���΃|���S����z��ɒǉ�
						if (wallNum < PLAYER_MAX_HITCOLL) {

							// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�
							wall[wallNum] = &hitDim[j].Dim[i];

							// �ǃ|���S���̐������Z
							++wallNum;
						}
					}
				}
				else {

					// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
					if (floorNum < PLAYER_MAX_HITCOLL) {

						// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
						floor[floorNum] = &hitDim[j].Dim[i];

						// ���|���S���̐������Z
						++floorNum;
					}
				}
			}
		}
	}

	// �ǃ|���S���Ƃ̓����蔻�菈��
	if (wallNum != 0) {

		// �ǂɓ����������ǂ����̃t���O��܂�
		hitFlag = false;

		// �ړ��������ǂ����ŏ����𕪊�
		if (moveFlag == true) {

			// �ǃ|���S���̐��J��Ԃ�
			for ( i = 0; i < wallNum; ++i)
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X���擾
				poly = wall[i];

				// �|���S���ƃv���C���[���Փ˂��Ă��Ȃ���Ύ��̃J�E���g��
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
					poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
					continue;

				// �����ɗ�����Փ˂����Ƃ������ƂȂ̂Ńt���O�𗧂Ă�
				hitFlag = true;

				// �ǂɓ���������ǂɎՂ��Ȃ��ړ������������ړ�
				VECTOR slideVec; // �X���C�h������x�N�g��

				// �i�s�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o
				slideVec = VCross(moveVec, poly->Normal);

				// �Z�o�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o
				// ���ꂪ���̈ړ���������Ǖ����̈ړ������𔲂����x�N�g��
				slideVec = VCross(poly->Normal, slideVec);

				// ���W��ݒ�
				nowPos = VAdd(oldPos, slideVec);

				// �ړ���̍��W�ŕǃ|���S���Ɠ������Ă��Ȃ����ǂ����𔻒肷��
				for (j = 0; j < wallNum; ++j) {

					// j�Ԗڂ̕ǃ|���S���̃A�h���X���擾
					poly = wall[j];

					// �������Ă����烋�[�v���甲����
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
						poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
						break;
				}

				// j �� wallNum �̏ꍇ�͂ǂ̃|���S���Ƃ�������Ȃ������Ƃ������ƂȂ̂�
				// ���[�v���甲����
				if (j == wallNum) {
					hitFlag = 0;
					break;
				}
			}
		}
		else
		{
			// �ړ����Ă��Ȃ��ꍇ�̏���

			// �ǃ|���S���̐��J��Ԃ�
			for (i = 0; i < wallNum; ++i) {

				// i�Ԗڂ̕ǃ|���S���̃A�h���X���擾
				poly = wall[i];

				// �������Ă����烋�[�v���甲����
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
				{
					hitFlag = true;
					break;
				}
			}
		}

		// �ǂɓ������Ă�����ǂ��牟���o���������s��
		if (hitFlag == true)
		{
			// �ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
			for (k = 0; k < PLAYER_HIT_TRYNUM; k++)
			{
				// �ǃ|���S���̐������J��Ԃ�
				for (i = 0; i < wallNum; i++)
				{
					// i�Ԗڂ̕ǃ|���S���̃A�h���X���擾
					poly = wall[i];

					// �v���C���[�Ɠ������Ă��邩�𔻒�
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
						poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
						continue;

					// �������Ă�����ǂ̖@�������Ɉړ�
					nowPos = VAdd(nowPos, VScale(poly->Normal, PLAYER_HIT_SLIDE_LENGTH));

					// �ړ�������ŕǃ|���S���ƐڐG���Ă��邩����
					for (j = 0; j < wallNum; j++)
					{
						// �������Ă����烋�[�v�𔲂���
						poly = wall[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH,
							poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
							break;
					}

					// �S�Ẵ|���S���Ɠ������Ă��Ȃ������烋�[�v�I��
					if (j == wallNum) break;
				}

				// i �� wallNum �ł͂Ȃ��ꍇ�͑S���̃|���S���ŉ����o�������݂�O�ɑS�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����Ƃ������ƂȂ̂Ń��[�v���甲����
				if (i != wallNum) break;
			}
		}
	}

	// ���|���S���Ƃ̓����蔻��
	if (floorNum != 0)
	{
		// �W�����v�����㏸���̏ꍇ�͏����𕪊�
		if (state == 2 && speed_y > 0.0f)
		{
			float MinY;

			// �V��ɓ����Ԃ��鏈�����s��

			// ��ԒႢ�V��ɂԂ���ׂ̔���p�ϐ���������
			MinY = 0.0f;

			// �����������ǂ����̃t���O��܂�
			hitFlag = false;

			// ���|���S���̐������J��Ԃ�
			for (i = 0; i < floorNum; i++)
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				poly = floor[i];

				// ���悩�瓪�̍����܂ł̊ԂŃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
				lineRes = HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);

				// �ڐG���Ă��Ȃ������牽�����Ȃ�
				if (lineRes.HitFlag == FALSE) continue;

				// ���Ƀ|���S���ɓ������Ă��āA�����܂Ō��o�����V��|���S����荂���ꍇ�͉������Ȃ�
				if (hitFlag == true && MinY < lineRes.Position.y) continue;

				// �|���S���ɓ��������t���O�𗧂Ă�
				hitFlag = true;

				// �ڐG�����x���W��ۑ�����
				MinY = lineRes.Position.y;
			}

			// �ڐG�����|���S�������������ǂ����ŏ����𕪊�
			if (hitFlag == true)
			{
				// �ڐG�����ꍇ�̓v���C���[�̂x���W��ڐG���W�����ɍX�V
				nowPos.y = MinY - PLAYER_HIT_HEIGHT;

				// �x�������̑��x�͔��]
				speed_y = -speed_y;
			}
		}
		else
		{
			float MaxY;

			// ���~�����W�����v���ł͂Ȃ��ꍇ�̏���

			// ���|���S���ɓ����������ǂ����̃t���O��|���Ă���
			hitFlag = false;

			// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
			MaxY = 0.0f;

			// ���|���S���̐������J��Ԃ�
			for (i = 0; i < floorNum; i++)
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				poly = floor[i];

				// �W�����v�����ǂ����ŏ����𕪊�
				if (state == 2)
				{
					// �W�����v���̏ꍇ�͓��̐悩�瑫���菭���Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -PLAYER_HIT_HEIGHT, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				else
				{
					// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(nowPos, VGet(0.0f, -PLAYER_HIT_HEIGHT, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}

				// �������Ă��Ȃ������牽�����Ȃ�
				if (lineRes.HitFlag == FALSE) continue;

				// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
				if (hitFlag == true && MaxY > lineRes.Position.y) continue;

				// �|���S���ɓ��������t���O�𗧂Ă�
				hitFlag = true;

				// �ڐG�����x���W��ۑ�����
				MaxY = lineRes.Position.y;
			}

			// ���|���S���ɓ����������ǂ����ŏ����𕪊�
			if (hitFlag == true)
			{
				// ���������ꍇ

				// �ڐG�����|���S���ň�ԍ����x���W���v���C���[�̂x���W�ɂ���
				nowPos.y = MaxY + PLAYER_HIT_HEIGHT;

				// �x�������̈ړ����x�͂O��
				speed_y = 0.0f;

				// �����W�����v���������ꍇ�͒��n��Ԃɂ���
				if (state == 2)
				{
					// �ړ����Ă������ǂ����Œ��n��̏�ԂƍĐ�����A�j���[�V�����𕪊򂷂�
					if (moveFlag)
					{
						// �ړ����Ă���ꍇ�͑����Ԃ�
						state = run;
					}
					else
					{
						// �ړ����Ă��Ȃ��ꍇ�͗����~���Ԃ�
						state = neutral;
					}
				}
			}
			else
			{
				// ���R���W�����ɓ������Ă��Ȃ��Ċ��W�����v��Ԃł͂Ȃ������ꍇ��
				if (state != 2)
				{
					// �W�����v���ɂ���
					state = jump;
				}
			}
		}
	}

	DrawFormatString(0, 0, GetColor(255, 255, 255), "x = %.3f", position.x);
	DrawFormatString(0, 15, GetColor(255, 255, 255), "y = %.3f", position.y);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "z = %.3f", position.z);
	DrawFormatString(0, 45, GetColor(255, 255, 255), "ModelHandle = %d", modelHandle);

	// �V�������W��ۑ�����
	position = nowPos;

	// �v���C���[�̃��f���̍��W���X�V����
	MV1SetPosition(modelHandle, position);

	// ���f����`��
	MV1DrawModel(modelHandle);

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	for (i = 0; i < hitDimNum; i++)
	{
		MV1CollResultPolyDimTerminate(hitDim[i]);
	}
}