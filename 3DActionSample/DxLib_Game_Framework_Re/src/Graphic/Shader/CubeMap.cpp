#include "CubeMap.h"
#include <DxLib.h>

// �N���X�F�L���[�u�}�b�v
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �f�X�g���N�^
CubeMap::~CubeMap()
{
	clear();	// �S�f�[�^������
}

// �ǂݍ���
bool CubeMap::load(int id, const std::string& file_name)
{
	// ����ID�̃f�[�^���폜
	erase(id);
	// ���f���f�[�^��ǂݍ���
	cube_map_[id] = LoadGraph(file_name.c_str());

	// �ǂݍ��݌��ʂ�Ԃ�
	return cube_map_[id] != -1;
}

// ���}�b�v�̍폜
void CubeMap::erase(int id)
{
	// �Y���̃V�F�[�_�[�f�[�^�����݂���ꍇ�A�f�[�^���폜
	if (cube_map_.count(id) > 0)
	{
		DeleteGraph(cube_map_[id]);
		cube_map_.erase(id);
	}
}

// �f�[�^�̏���
void CubeMap::clear()
{
	// �S�ẴV�F�[�_�[�f�[�^������
	for (const auto data : cube_map_)
	{
		DeleteGraph(data.second);
	}
	cube_map_.clear();
}

// �n���h���̎擾
int CubeMap::operator[](int id) const
{
	return cube_map_.at(id);
}