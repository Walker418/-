#ifndef GRAPHICS_3D_H_
#define GRAPHICS_3D_H_

#include "Vector3.h"
#include "Color.h"
#include "Matrix.h"

// �\���́F���C�g�i�����j
struct Light
{
	// �񋓌^�F���C�g�̎��
	enum class LightType
	{
		Directional,	// ���s����
		Point,			// �|�C���g���C�g
		Spot,			// �X�|�b�g���C�g
	};

	LightType	type{ LightType::Directional };		// ���C�g�̎��
	Vector3		direction{ 1.0f, -1.0f, 1.0f };		// ���C�g�̕���
	Vector3		position{ 0.0f, 0.0f, 0.0f };		// ���C�g�̍��W
	Color		ambient{ 0.3f, 0.3f, 0.3f, 1.0f };	// ����
	Color		diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };	// �g�U���ˌ�
	Color		specular{ 1.0f, 1.0f, 1.0f ,1.0f };	// ���ʔ��ˌ�
	float		attenuation0{ 0.1f };				// �����p�����[�^1
	float		attenuation1{ 0.1f };				// �����p�����[�^2
	float		attenuation2{ 0.1f };				// �����p�����[�^3
	float		range{ 2000.0f };					// ���C�g�̗L������
	float		out_angle{ 114.0f };				// �X�|�b�g���C�g�̊O���p�x
	float		in_angle{ 114.0f };					// �X�|�b�g���C�g�̓����p�x
};

// �񋓌^�F�u�����h���[�h
enum class BlendMode
{
	None,		// ����
	Alpha,		// �A���t�@�u�����h
	Add,		// ���Z�u�����h
	Sub,		// ���Z�u�����h
};

// �N���X�F3D�O���t�B�b�N�X
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Graphics3D
{
public:
	// ������
	static void initialize();
	// �I��
	static void finalize();

	// �X�N���[���̏���
	static void clear();
	// �X�N���[�������J���[�̐ݒ�
	static void clear_color(float r, float g, float b);

	// �r���[�|�[�g�̐ݒ�
	static void set_viewport(int x, int y, int w, int h);
	// ����ϊ��s��̐ݒ�
	static void set_view_matrix(const Matrix& view);
	// �����ϊ��s��̐ݒ�
	static void set_projection_matrix(const Matrix& projection);
	// ����ϊ��s��̎擾
	static Matrix get_view_matrix();
	// �����ϊ��s��̎擾
	static Matrix get_projection_matrix();

	// ���C�g�̐ݒ�
	static void set_light(const Light& light);
	// ���C�g�̎擾
	static const Light& get_light();
	// ���C�e�B���O�̗L����
	static void enable_lighting();
	// ���C�e�B���O�̖�����
	static void disable_lighting();
	// Z�o�b�t�@�̗L����
	static void enable_z_buffer();
	// Z�o�b�t�@�̖�����
	static void disable_z_buffer();
	// Z�o�b�t�@�ւ̏������݂̋���
	static void enable_write_z_buffer();
	// Z�o�b�t�@�ւ̏������݂̋֎~
	static void disable_write_z_buffer();

	// �u�����h���[�h�̐ݒ�
	static void blend_mode(BlendMode mode, float a = 1.0f);

private:
	// ���C�g�̃p�����[�^
	static Light light_;
};

#endif // !GRAPHICS_3D_H_