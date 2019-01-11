#include "Skybox.h"
#include <DxLib.h>

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
	// Z�o�b�t�@�ɏ������݂��s��Ȃ�
	MV1SetWriteZBuffer(model_, FALSE);
	// ���f����\��
	MV1DrawModel(model_);
	// ����ϊ��s������ɖ߂�
	SetCameraViewMatrix(view_matrix);
	// ���C�e�B���O��L����
	SetUseLighting(TRUE);
}