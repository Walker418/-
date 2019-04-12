#ifndef MODEL_ASSET_H_
#define MODEL_ASSET_H_

#include <string>
#include <unordered_map>

// �N���X�F���f���A�Z�b�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class ModelAsset
{
public:
	// �f�t�H���g�R���X�g���N�^
	ModelAsset() = default;
	// �f�X�g���N�^
	~ModelAsset();
	// ���f���̓ǂݍ���
	bool load(int id, const std::string& file_name);
	// ���f���̍폜
	void erase(int id);
	// �f�[�^�̏���
	void clear();
	// �n���h���̎擾
	int operator[](int id) const;

	// �R�s�[�֎~
	ModelAsset(const ModelAsset& other) = delete;
	ModelAsset& operator = (const ModelAsset& other) = delete;

private:
	// ���f���f�[�^�}�b�v
	std::unordered_map<int, int> asset_;
};

#endif // !MODEL_ASSET_H_