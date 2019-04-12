#include "Shader.h"
#include <DxLib.h>

// �N���X�F�V�F�[�_�[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �f�X�g���N�^
Shader::~Shader()
{
	clear();	// �S�f�[�^������
}

// ���_�V�F�[�_�[�̓ǂݍ���
bool Shader::load_vs(int id, const std::string & file_name)
{
	// ����ID�̃f�[�^���폜
	erase(id);
	// ���f���f�[�^��ǂݍ���
	shader_[id] = LoadVertexShader(file_name.c_str());

	// �ǂݍ��݌��ʂ�Ԃ�
	return shader_[id] != -1;
}

// �s�N�Z���V�F�[�_�[�̓ǂݍ���
bool Shader::load_ps(int id, const std::string & file_name)
{
	// ����ID�̃f�[�^���폜
	erase(id);
	// ���f���f�[�^��ǂݍ���
	shader_[id] = LoadPixelShader(file_name.c_str());

	// �ǂݍ��݌��ʂ�Ԃ�
	return shader_[id] != -1;
}

// �V�F�[�_�[�̍폜
void Shader::erase(int id)
{
	// �Y���̃V�F�[�_�[�f�[�^�����݂���ꍇ�A�f�[�^���폜
	if (shader_.count(id) > 0)
	{
		DeleteShader(shader_[id]);
		shader_.erase(id);
	}
}

// �f�[�^�̏���
void Shader::clear()
{
	// �S�ẴV�F�[�_�[�f�[�^������
	for (const auto data : shader_)
	{
		DeleteShader(data.second);
	}
	shader_.clear();
}

// �n���h���̎擾
int Shader::operator[](int id) const
{
	return shader_.at(id);
}

// ���_�V�F�[�_�[�̎w��
void Shader::set_vs(int id)
{
	SetUseVertexShader(shader_[id]);
}

// �s�N�Z���V�F�[�_�[�̎w��
void Shader::set_ps(int id)
{
	SetUsePixelShader(shader_[id]);
}