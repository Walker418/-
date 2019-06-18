#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

#include <string>
#include <unordered_map>

#include "Shader.h"
#include "CubeMap.h"

// �N���X�F�V�F�[�_�[�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class ShaderManager
{
public:
	// ������
	static void initialize();
	// �I������
	static void finalize();
	// ���_�V�F�[�_�[�̓ǂݍ���
	static bool load_vs(int id, const std::string& file_name);
	// �s�N�Z���V�F�[�_�[�̓ǂݍ���
	static bool load_ps(int id, const std::string& file_name);
	// �L���[�u�}�b�v�̓ǂݍ���
	static bool load_cubemap(int id, const std::string& file_name);
	// ���_�V�F�[�_�[�̍폜
	static void erase_vs(int id);
	// �s�N�Z���V�F�[�_�[�̍폜
	static void erase_ps(int id);
	// �L���[�u�}�b�v�̍폜
	static void erase_cubemap(int id);
	// ���_�V�F�[�_�[�̎w��
	static void set_vs(int id);
	// �s�N�Z���V�F�[�_�[�̎w��
	static void set_ps(int id);
	// �L���[�u�}�b�v�̎w��
	static void set_cubemap(int id, int stage);
	// �L���[�u�}�b�v�̉���
	static void off_cubemap(int stage);

	// �V�F�[�_�[�̗L����
	static void shader_on();
	// �V�F�[�_�[�̖�����
	static void shader_off();

private:
	// �V�F�[�_�[�A�Z�b�g
	static Shader	shader_asset_;
	// �L���[�u�}�b�v�A�Z�b�g
	static CubeMap	cubemap_asset_;
	// �s�N�Z���V�F�[�_�[�p�̕ϐ��o�b�t�@
	static int		pscbhandle_;
};

#endif // !SHADER_MANAGER_H_