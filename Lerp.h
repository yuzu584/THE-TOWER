#pragma once

#include "DxLib.h"

class LERP
{
public:
	void Lerp_Float(float num);   // float�^�̐��`�⊮
	void Lerp_VECTOR(VECTOR vec); // VECTOR�^�̐��`�⊮
};

extern LERP lerp;