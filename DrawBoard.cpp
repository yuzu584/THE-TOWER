#include "DrawBoard.h"

void DrawBoard(VECTOR TopLeft, VECTOR BottomRight, int *graphHandle)
{
	VERTEX3D vertex[4];
	WORD index[6];
	// �S���_���̃f�[�^���Z�b�g

	// ����
	vertex[0].pos = TopLeft;
	vertex[0].norm = VGet(0.0f, 0.0f, -1.0f);
	vertex[0].dif = GetColorU8(255, 255, 255, 255);
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	// �E��
	vertex[1].pos = VGet(BottomRight.x, TopLeft.y, BottomRight.z);
	vertex[1].norm = VGet(0.0f, 0.0f, -1.0f);
	vertex[1].dif = GetColorU8(255, 255, 255, 255);
	vertex[1].spc = GetColorU8(0, 0, 0, 0);
	vertex[1].u = 1.0f;
	vertex[1].v = 0.0f;
	vertex[1].su = 1.0f;
	vertex[1].sv = 0.0f;

	// ����
	vertex[2].pos = VGet(TopLeft.x, BottomRight.y, TopLeft.z);
	vertex[2].norm = VGet(0.0f, 0.0f, -1.0f);
	vertex[2].dif = GetColorU8(255, 255, 255, 255);
	vertex[2].spc = GetColorU8(0, 0, 0, 0);
	vertex[2].u = 0.0f;
	vertex[2].v = 1.0f;
	vertex[2].su = 0.0f;
	vertex[2].sv = 1.0f;

	// �E��
	vertex[3].pos = BottomRight;
	vertex[3].norm = VGet(0.0f, 0.0f, -1.0f);
	vertex[3].dif = GetColorU8(255, 255, 255, 255);
	vertex[3].spc = GetColorU8(0, 0, 0, 0);
	vertex[3].u = 1.0f;
	vertex[3].v = 1.0f;
	vertex[3].su = 1.0f;
	vertex[3].sv = 1.0f;

	// �Q�|���S�����̃C���f�b�N�X�f�[�^���Z�b�g
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	// �Q�|���S���̕`��
	DrawPolygonIndexed3D(vertex, 4, index, 2, *graphHandle, false);
}