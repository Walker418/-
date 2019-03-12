#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

// �N���X�F�v�Z�⏕
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class MathHelper
{
public:
	// �~�����̒萔
	static const float PI;

	// ���W�A�����p�x�ɕϊ�
	static float radian_to_degree(float radian);
	// �p�x�����W�A���ɕϊ�
	static float degree_to_radian(float degree);
	// ���`���
	static float lerp(float start, float end, float t);
	// ���l�𐧌�
	static float clamp(float value, float min, float max);

	// �T�C�������߂�
	static float sin(float degree);
	// �R�T�C�������߂�
	static float cos(float degree);
	// �^���W�F���g�����߂�
	static float tan(float degree);
	// �T�C������p�x�֋t�Z
	static float a_sin(float s);
	// �R�T�C������p�x�֋t�Z
	static float a_cos(float c);
	// �^���W�F���g����p�x�֋t�Z
	static float a_tan(float y, float x);
};

#endif // !MATH_HELPER_H_