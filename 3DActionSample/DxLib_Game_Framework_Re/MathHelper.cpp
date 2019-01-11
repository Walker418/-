#include "MathHelper.h"
#include <cmath>
#include <algorithm>

// �N���X�F�v�Z�⏕
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �~�����̎��̂��`
const float MathHelper::PI{ 3.14159265358979323846264f };

// ���W�A�����p�x�ɕϊ�
float MathHelper::radian_to_degree(float radian)
{
	return radian * (180.0f / PI);
}

// �p�x�����W�A���ɕϊ�
float MathHelper::degree_to_radian(float degree)
{
	return degree * (PI / 180.0f);
}

// ���`���
float MathHelper::lerp(float start, float end, float t)
{
	return start * (1.0f - t) + end * t;
}

// ���l�𐧌�
float MathHelper::clamp(float value, float min, float max)
{
	return (value < min) ? min : (value > max) ? max : value;
}

// �T�C�������߂�
float MathHelper::sin(float degree)
{
	return std::sin(degree_to_radian(degree));
}

// �R�T�C�������߂�
float MathHelper::cos(float degree)
{
	return std::cos(degree_to_radian(degree));
}

// �^���W�F���g�����߂�
float MathHelper::tan(float degree)
{
	return std::tan(degree_to_radian(degree));
}

// �T�C������p�x�֋t�Z
float MathHelper::a_sin(float s)
{
	return radian_to_degree(std::asin(s));
}

// �R�T�C������p�x�֋t�Z
float MathHelper::a_cos(float c)
{
	return radian_to_degree(std::acos(c));
}

// �^���W�F���g����p�x�֋t�Z
float MathHelper::a_tan(float y, float x)
{
	return radian_to_degree(std::atan2(y, x));
}