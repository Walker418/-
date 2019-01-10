#include "TextureAsset.h"
#include <DxLib.h>

// �N���X�F�e�N�X�`���A�Z�b�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �f�X�g���N�^
TextureAsset::~TextureAsset()
{
	// �f�[�^������
	clear();
}

// �e�N�X�`���̓ǂݍ���
bool TextureAsset::load(int id, const std::string& file_name)
{
	// ����ID�̃f�[�^���폜
	erase(id);
	// ���f���f�[�^��ǂݍ���
	textures_[id] = LoadGraph(file_name.c_str());

	// �ǂݍ��݌��ʂ�Ԃ�
	return textures_[id] != -1;
}

// �e�N�X�`���̍폜
void TextureAsset::erase(int id)
{
	// �Y���̃e�N�X�`���f�[�^�����݂���ꍇ�A�f�[�^���폜
	if (textures_.count(id) > 0)
	{
		DeleteGraph(textures_[id]);
		textures_.erase(id);
	}
}

// �f�[�^�̏���
void TextureAsset::clear()
{
	// �S�Ẵe�N�X�`���f�[�^������
	for (const auto texture : textures_)
	{
		DeleteGraph(texture.second);
	}

	textures_.clear();
}

// �n���h���̎擾
int TextureAsset::operator[](int id) const
{
	return textures_.at(id);
}
