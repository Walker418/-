#ifndef STATIC_MESH_H_
#define STATIC_MESH_H_

#include <string>
#include "Matrix.h"
#include "ModelAsset.h"

// �N���X�F�X�^�e�B�b�N���b�V��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class StaticMesh
{
public:
	// ������
	static void initialize();
	// �I������
	static void finalize();
	// �ǂݍ���
	static bool load(int id, const std::string& file_name);
	// �폜
	static void erase(int id);
	// ���b�V���̃o�C���h
	static void bind(int id);
	// �`��
	static void draw();
	// ���[���h�ϊ��s��̌v�Z
	static void transform(const Matrix& world);

private:
	// �o�C���h���̃��f��
	static int			model_;
	// �{�[���̃��[���h�ϊ��s��
	static Matrix		world_matrix_;
	// ���f���A�Z�b�g
	static ModelAsset	asset_;
};

#endif // !STATIC_MESH_H_