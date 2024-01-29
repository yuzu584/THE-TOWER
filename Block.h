#pragma once

#include "DxLib.h"
#include <string>
#include <fstream>

constexpr int BLOCK_TYPE_AMOUNT = 4;                 // �u���b�N�̎�ނ̐�
constexpr VECTOR BLOCK_SCALE = { 0.5f, 0.5f, 0.5f }; // �u���b�N�̃X�P�[��

class Block {
private:
	int id = 0;                   // �u���b�NID
	std::string name = "";        // ���O
	int direction = 0;            // ����(0:�O 1:�E 2:��� 3:�� 4:�� 5:��)
	int modelHandle = 0;          // ���f���n���h��
public:
	void SetData(int num);   // �u���b�N�̃f�[�^��������
	void Render(VECTOR pos); // �u���b�N�̃��f����`��

	Block();

	int GetId();
	std::string GetName();
	int GetDirection();
	void SetDirection(int num);
};