#ifndef CUBE_MAP_H_
#define CUBE_MAP_H_

#include <string>
#include <unordered_map>

// �N���X�F�L���[�u�}�b�v
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class CubeMap
{
public:
	// �f�t�H���g�R���X�g���N�^
	CubeMap() = default;
	// �f�X�g���N�^
	~CubeMap();
	// �ǂݍ���
	bool load(int id, const std::string& file_name);
	// ���}�b�v�̍폜
	void erase(int id);
	// �f�[�^�̏���
	void clear();
	// �n���h���̎擾
	int operator[](int id) const;

	// �R�s�[�֎~
	CubeMap(const CubeMap& other) = delete;
	CubeMap& operator = (const CubeMap& other) = delete;

private:
	// ���}�b�v�f�[�^�}�b�v
	std::unordered_map<int, int> cube_map_;
};

#endif // !CUBE_MAP_H_