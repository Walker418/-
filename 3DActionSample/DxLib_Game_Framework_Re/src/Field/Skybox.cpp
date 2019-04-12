#include "Skybox.h"
#include <DxLib.h>

#include "../Graphic/Shader/ShaderManager.h"
#include "../ID/SourceID.h"

// �N���X�F�X�^�e�B�b�N���b�V��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �o�C���h���̃��f��
int Skybox::model_{ -1 };
// ���f���A�Z�b�g
ModelAsset Skybox::asset_;

// ������
void Skybox::initialize()
{
	// �I�������Ɠ���
	finalize();
}

// �I������
void Skybox::finalize()
{
	// ���f���A�Z�b�g������
	asset_.clear();
	// ���f��������
	model_ = -1;
}

// �ǂݍ���
bool Skybox::load(int id, const std::string& file_name)
{
	return asset_.load(id, file_name);
}

// �폜
void Skybox::erase(int id)
{
	model_ = (model_ == asset_[id]) ? -1 : model_;
	asset_.erase(id);
}

// ���b�V���̃o�C���h
void Skybox::bind(int id)
{
	model_ = asset_[id];
}

// �`��
void Skybox::draw()
{
	// ����ϊ��s����擾
	MATRIX view_matrix = GetCameraViewMatrix();
	// ����ϊ��s��̕��s�ړ��ʂ��N���A
	MATRIX skybox_matrix = view_matrix;
	skybox_matrix.m[3][0] = 0.0f;
	skybox_matrix.m[3][1] = 0.0f;
	skybox_matrix.m[3][2] = 0.0f;
	// ����ϊ��s���ݒ�
	SetCameraViewMatrix(skybox_matrix);
	// ���C�e�B���O�𖳌���
	SetUseLighting(FALSE);

	// �I���W�i���V�F�[�_�[��L����
	ShaderManager::shader_on();
	// �X�J�C�{�b�N�X�p���_�V�F�[�_�[��ݒ�
	ShaderManager::set_vs(SHADER_SKYBOX_VERTEX_SHADER);
	// �X�J�C�{�b�N�X�p�s�N�Z���V�F�[�_�[��ݒ�
	ShaderManager::set_ps(SHADER_SKYBOX_PIXEL_SHADER);

	// Z�o�b�t�@�ɏ������݂��s��Ȃ�
	MV1SetWriteZBuffer(model_, FALSE);

	// ���}�b�v��ݒ�
	ShaderManager::set_cubemap(CUBEMAP_TEST_ENV_HDR, 11);

	// ���f����\��
	MV1DrawModel(model_);

	// �I���W�i���V�F�[�_�[�𖳌���
	ShaderManager::shader_off();
	// �e�N�X�`��������
	ShaderManager::off_cubemap(11);

	// ����ϊ��s������ɖ߂�
	SetCameraViewMatrix(view_matrix);
	// ���C�e�B���O��L����
	SetUseLighting(TRUE);
}