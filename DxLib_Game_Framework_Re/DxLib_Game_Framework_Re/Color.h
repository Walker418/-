#ifndef COLOR_H_
#define COLOR_H_

// �\���́F�J���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
struct Color
{
	float r{ 1.0f };	// �Ԑ���
	float g{ 1.0f };	// �ΐ���
	float b{ 1.0f };	// ����
	float a{ 1.0f };	// �A���t�@����

	// �f�t�H���g�R���X�g���N�^
	Color() = default;
	// �R���X�g���N�^
	Color(float r, float g, float b, float a);

	// ���`���
	static Color Lerp(const Color& v1, const Color& v2, float amount);
	// ��v����e�����y�A�̍ł��Ⴂ�l���܂ރJ���[�����߂�
	static Color Min(const Color& v1, const Color& v2);
	// ��v����e�����y�A�̍ł������l���܂ރJ���[�����߂�
	static Color Max(const Color& v1, const Color& v2);
	// ���l�𐧌�
	static Color Clamp(const Color& value, const Color& min = Color(0.0f, 0.0f, 0.0f, 0.0f), const Color& max = Color(1.0f, 1.0f, 1.0f, 1.0f));

	// �萔
	static const Color Black;	// Color(0, 0, 0, 1)
	static const Color White;   // Color(1, 1, 1, 1)
};

// ���Z�q�I�[�o�[���[�h
Color& operator += (Color& c1, const Color& c2);
Color& operator -= (Color& c1, const Color& c2);
Color& operator *= (Color& c1, const Color& c2);
Color& operator *= (Color& c, float s);
Color& operator /= (Color& c, float s);

Color operator + (Color c1, const Color& c2);
Color operator - (Color c1, const Color& c2);
Color operator * (Color c1, const Color& c2);
Color operator * (Color c, float s);
Color operator * (float s, Color c);
Color operator / (Color c, float s);

#endif // !COLOR_H_