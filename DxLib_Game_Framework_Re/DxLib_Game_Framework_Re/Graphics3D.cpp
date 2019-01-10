#include "Graphics3D.h"
#include "MathHelper.h"

// �N���X�F3D�O���t�B�b�N�X
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// ���C�g�̃p�����[�^
Light Graphics3D::light_;

// ������
void Graphics3D::initialize()
{
	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);
	// Z�o�b�t�@��L����
	enable_z_buffer();
	// Z�o�b�t�@�ւ̏������݂�����
	enable_write_z_buffer();
	// �f�t�H���g�̃��C�g�ݒ�
	set_light(Light());
	// ���C�e�B���O��L����
	enable_lighting();
}

// �I��
void Graphics3D::finalize()
{

}

// �X�N���[���̏���
void Graphics3D::clear()
{
	ClearDrawScreen();
}

// �X�N���[�������J���[�̐ݒ�
void Graphics3D::clear_color(float r, float g, float b)
{
	SetBackgroundColor(int(r * 255), int(g * 255), int(b * 255));
}

// �r���[�|�[�g�̐ݒ�
void Graphics3D::set_viewport(int x, int y, int w, int h)
{
	MATRIX viewport_matrix;
	CreateViewportMatrix(&viewport_matrix, x + (w / 2.0f), y + (h / 2.0f), float(w), float(h));
	SetTransformToViewport(&viewport_matrix);
	SetDrawArea(x, y, x + w, y + h);
}

// ����ϊ��s��̐ݒ�
void Graphics3D::set_view_matrix(const Matrix& view)
{
	SetCameraViewMatrix(view);
}

// �����ϊ��s��̐ݒ�
void Graphics3D::set_projection_matrix(const Matrix& projection)
{
	SetupCamera_ProjectionMatrix(projection);
}

// ����ϊ��s��̎擾
Matrix Graphics3D::get_view_matrix()
{
	return GetCameraViewMatrix();
}

// �����ϊ��s��̎擾
Matrix Graphics3D::get_projection_matrix()
{
	return GetCameraProjectionMatrix();
}

// ���C�g�̐ݒ�
void Graphics3D::set_light(const Light& light)
{
	switch (light.type)
	{
	case Light::LightType::Directional:
		ChangeLightTypeDir(light.direction);
		break;
	case Light::LightType::Point:
		ChangeLightTypePoint(light.position, light.range, light.attenuation0, light.attenuation1, light.attenuation2);
		break;
	case Light::LightType::Spot:
		ChangeLightTypeSpot(light.position, light.direction,
			MathHelper::degree_to_radian(light.out_angle), MathHelper::degree_to_radian(light.in_angle),
			light.range, light.attenuation0, light.attenuation1, light.attenuation2);
		break;
	}

	SetLightDifColor(GetColorF(light.diffuse.r, light.diffuse.g, light.diffuse.b, light.diffuse.a));
	SetLightSpcColor(GetColorF(light.specular.r, light.specular.g, light.specular.b, light.specular.a));
	SetLightAmbColor(GetColorF(light.ambient.r, light.ambient.g, light.ambient.b, light.ambient.a));
	light_ = light;
}

// ���C�g�̎擾
const Light& Graphics3D::get_light()
{
	return light_;
}

// ���C�e�B���O�̗L����
void Graphics3D::enable_lighting()
{
	SetLightEnable(TRUE);
}

// ���C�e�B���O�̖�����
void Graphics3D::disable_lighting()
{
	SetLightEnable(FALSE);
}

// Z�o�b�t�@�̗L����
void Graphics3D::enable_z_buffer()
{
	SetUseZBuffer3D(TRUE);
}

// Z�o�b�t�@�̖�����
void Graphics3D::disable_z_buffer()
{
	SetUseZBuffer3D(FALSE);
}

// Z�o�b�t�@�ւ̏������݂̋���
void Graphics3D::enable_write_z_buffer()
{
	SetWriteZBuffer3D(TRUE);
}

// Z�o�b�t�@�ւ̏������݂̋֎~
void Graphics3D::disable_write_z_buffer()
{
	SetWriteZBuffer3D(FALSE);
}

// �u�����h���[�h�̐ݒ�
void Graphics3D::blend_mode(BlendMode mode, float a)
{
	switch (mode)
	{
	case BlendMode::None:
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	case BlendMode::Alpha:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(a * 255));
		break;
	case BlendMode::Add:
		SetDrawBlendMode(DX_BLENDMODE_ADD, int(a * 255));
		break;
	case BlendMode::Sub:
		SetDrawBlendMode(DX_BLENDMODE_SUB, int(a * 255));
		break;
	}
}