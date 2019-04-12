#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <unordered_map>

// �N���X�F�V�F�[�_�[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Shader
{
public:
	// �f�t�H���g�R���X�g���N�^
	Shader() = default;
	// �f�X�g���N�^
	~Shader();
	// ���_�V�F�[�_�[�̓ǂݍ���
	bool load_vs(int id, const std::string& file_name);
	// �s�N�Z���V�F�[�_�[�̓ǂݍ���
	bool load_ps(int id, const std::string& file_name);
	// �V�F�[�_�[�̍폜
	void erase(int id);
	// �f�[�^�̏���
	void clear();
	// �n���h���̎擾
	int operator[](int id) const;
	// ���_�V�F�[�_�[�̎w��
	void set_vs(int id);
	// �s�N�Z���V�F�[�_�[�̎w��
	void set_ps(int id);

	// �R�s�[�֎~
	Shader(const Shader& other) = delete;
	Shader& operator = (const Shader& other) = delete;

private:
	// �V�F�[�_�[�f�[�^�}�b�v
	std::unordered_map<int, int> shader_;
};

#endif // !SHADER_H_