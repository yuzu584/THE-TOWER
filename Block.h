#pragma once

#include "DxLib.h"
#include <string>
#include <fstream>

constexpr int BLOCK_TYPE_AMOUNT = 6;                 // �u���b�N�̎�ނ̐�
constexpr VECTOR BLOCK_SCALE = { 0.5f, 0.5f, 0.5f }; // �u���b�N�̃X�P�[��
constexpr VECTOR BLOCK_DIRECTION[4] = {              // �u���b�N�̊p�x�̔z��
	{0.0f, 270.0f * DX_PI_F / 180.0f, 0.0f}, // �E
	{0.0f, 0.0f, 0.0f},                      // �O
	{0.0f, 90.0f * DX_PI_F / 180.0f, 0.0f},  // ��
	{0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f}, // ���
};

class BLOCK {
private:
	int id = 0;                   // �u���b�NID
	std::string name = "";        // ���O
	int direction = 0;            // ����(0:�E 1:�O 2:�� 3:���)
	int modelHandle = 0;          // ���f���n���h��
public:
	void SetData(int num);   // �u���b�N�̃f�[�^��������
	void Render(VECTOR pos); // �u���b�N�̃��f����`��
	void Initialize(); // �u���b�N��������

	BLOCK();

	int GetId();
	std::string GetName();
	int GetDirection();
	void SetDirection(int num);
	int GetModelHandle() { return modelHandle; } // ���f���n���h�����擾
};