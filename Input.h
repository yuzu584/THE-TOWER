#pragma once

// ���͏��̃N���X
class PADINPUT {
private:
	int nowInput;  // ���݂̓��͒l
	int edgeInput; // ���݂̃t���[���ŉ����ꂽ�{�^���̂݃r�b�g�������Ă�����͒l
public:
	void Process();                        // ���͏���
	int GetNowInput() { return nowInput; } // ���݂̓��͒l���擾
	int GetEdgeInput() { return edgeInput; } // ���݂̃t���[���ŉ����ꂽ�{�^���̓��͒l���擾
};

extern PADINPUT input; // ���͏��