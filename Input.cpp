#include "DxLib.h"
#include "Input.h"

PADINPUT input; // ���͏���錾

// ���͏���
void PADINPUT::Process() {
	int old;

	// 1�O�̃t���[���̓��͂�ۑ�
	old = nowInput;

	// ���݂̓��͏�Ԃ��擾
	nowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ���̃t���[���ŐV���ɉ����ꂽ�{�^���̃r�b�g���������Ă���l�� edgeInput �ɑ������
	edgeInput = nowInput & ~old;
}