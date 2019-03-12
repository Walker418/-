#include "ModelAsset.h"
#include <DxLib.h>

// �N���X�F���f���A�Z�b�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �f�X�g���N�^
ModelAsset::~ModelAsset()
{
	// �f�[�^������
	clear();
}

// ���f���̓ǂݍ���
bool ModelAsset::load(int id, const std::string& file_name)
{
	// ����ID�̃f�[�^���폜
	erase(id);
	// ���f���f�[�^��ǂݍ���
	asset_[id] = MV1LoadModel(file_name.c_str());

	// �ǂݍ��݌��ʂ�Ԃ�
	return asset_[id] != -1;
}

// ���f���̍폜
void ModelAsset::erase(int id)
{
	// �Y���̃��f���f�[�^�����݂���ꍇ�A�f�[�^���폜
	if (asset_.count(id) > 0)
	{
		MV1DeleteModel(asset_[id]);
		asset_.erase(id);
	}
}

// �f�[�^�̏���
void ModelAsset::clear()
{
	// �S�Ẵ��f���f�[�^������
	for (const auto model : asset_)
	{
		MV1DeleteModel(model.second);
	}
	asset_.clear();
}

// �n���h���̎擾
int ModelAsset::operator[](int id) const
{
	return asset_.at(id);
}