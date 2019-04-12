#include "ShaderManager.h"
#include <DxLib.h>

#include "PixelShaderCB.h"
#include "../../Actor/ActorManager.h"
#include "../../Actor/ActorGroup.h"

// �N���X�F�V�F�[�_�[�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �V�F�[�_�[�A�Z�b�g
Shader ShaderManager::shader_asset_;
// �L���[�u�}�b�v�A�Z�b�g
CubeMap ShaderManager::cubemap_asset_;
// �s�N�Z���V�F�[�_�[�p�̕ϐ��o�b�t�@
int ShaderManager::pscbhandle_{ -1 };

// ������
void ShaderManager::initialize()
{
	// �I�������Ɠ���
	finalize();

	// �s�N�Z���V�F�[�_�[�p�̕ϐ��o�b�t�@���쐬
	pscbhandle_ = CreateShaderConstantBuffer(sizeof(PixelShaderCB));
}

// �I������
void ShaderManager::finalize()
{
	shader_asset_.clear();
	cubemap_asset_.clear();
	pscbhandle_ = -1;
}

// ���_�V�F�[�_�[�̓ǂݍ���
bool ShaderManager::load_vs(int id, const std::string& file_name)
{
	return shader_asset_.load_vs(id, file_name);
}

// �s�N�Z���V�F�[�_�[�̓ǂݍ���
bool ShaderManager::load_ps(int id, const std::string& file_name)
{
	return shader_asset_.load_ps(id, file_name);
}

// �L���[�u�}�b�v�̓ǂݍ���
bool ShaderManager::load_cubemap(int id, const std::string& file_name)
{
	return cubemap_asset_.load(id, file_name);
}

// ���_�V�F�[�_�[�̍폜
void ShaderManager::erase_vs(int id)
{
	shader_asset_.erase(id);
}

// �s�N�Z���V�F�[�_�[�̍폜
void ShaderManager::erase_ps(int id)
{
	shader_asset_.erase(id);
}

// �L���[�u�}�b�v�̍폜
void ShaderManager::erase_cubemap(int id)
{
	cubemap_asset_.erase(id);
}

// ���_�V�F�[�_�[�̎w��
void ShaderManager::set_vs(int id)
{
	shader_asset_.set_vs(id);
}

// �s�N�Z���V�F�[�_�[�̎w��
void ShaderManager::set_ps(int id)
{
	shader_asset_.set_ps(id);
}

// �L���[�u�}�b�v�̎w��
void ShaderManager::set_cubemap(int id, int stage)
{
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP, stage);
	SetUseTextureToShader(stage, cubemap_asset_[id]);
}

// �L���[�u�}�b�v�̉���
void ShaderManager::off_cubemap(int stage)
{
	SetUseTextureToShader(11, -1);
}

// �X�V
void ShaderManager::update(IWorld *world, float delta_time)
{
	// �J�����̎Q�Ƃ��擾���Ă���
	auto camera = world->camera();
	// ���C�g�̎Q�Ƃ��擾���Ă���
	auto light = world->light();

	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�̃A�h���X���擾
	auto cb = (PixelShaderCB*)GetBufferShaderConstantBuffer(pscbhandle_);
	// �J�����̍��W��ݒ�
	cb->ViewPosition = camera->position();
	// ���C�g�̍��W��ݒ�
	cb->LightPosition = light->position();
	// ���C�g�̃J���[��ݒ�
	cb->LightColor = { 1.0f, 1.0f, 1.0f };
	// �萔�o�b�t�@�̓��e���X�V
	UpdateShaderConstantBuffer(pscbhandle_);
}

// �V�F�[�_�[�̗L����
void ShaderManager::shader_on()
{
	MV1SetUseOrigShader(TRUE);
}

// �V�F�[�_�[�̖�����
void ShaderManager::shader_off()
{
	MV1SetUseOrigShader(FALSE);
}