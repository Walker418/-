#ifndef TEXTURE_ASSET_H_
#define TEXTURE_ASSET_H_

#include <unordered_map>
#include <string>

// �N���X�F�e�N�X�`���A�Z�b�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class TextureAsset
{
public:
	// �f�t�H���g�R���X�g���N�^
	TextureAsset() = default;
	// �f�X�g���N�^
	~TextureAsset();
	// �e�N�X�`���̓ǂݍ���
	bool load(int id, const std::string& file_name);
	// �e�N�X�`���̍폜
	void erase(int id);
	// �f�[�^�̏���
	void clear();
	// �n���h���̎擾
	int operator[](int id) const;

	// �R�s�[�֎~
	TextureAsset(const TextureAsset& other) = delete;
	TextureAsset& operator = (const TextureAsset& other) = delete;

private:
	// �e�N�X�`���f�[�^�}�b�v
	std::unordered_map<int, int> textures_;
};

#endif // !TEXTURE_ASSET_H_