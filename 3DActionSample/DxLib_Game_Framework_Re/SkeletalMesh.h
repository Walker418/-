#ifndef SKELETAL_MESH_H_
#define SKELETAL_MESH_H_

#include <string>
#include "Matrix.h"
#include "ModelAsset.h"

// �N���X�F�X�P���^�����b�V��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class SkeletalMesh
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
	// �A�j���[�V�����̃o�C���h
	static void bind_animation(int motion, float time);
	// �A�j���[�V�����̃o�C���h�i��ԕt���j
	static void bind_animation(int prev_motion, float prev_time, int motion, float time, float amount);
	// ���[���h�ϊ��s��̌v�Z
	static void transform(const Matrix& world);
	// �`��
	static void draw();
	// ���[�J���ϊ��s��̎擾
	static void get_local_matrices(Matrix local_matrices[]);
	// ���[�J���ϊ��s��̐ݒ�
	static void set_local_matrices(const Matrix local_matrices[]);
	// ���[���h�ϊ��s��̎擾
	static void get_world_matrices(Matrix world_matrices[]);
	// ���[���h�ϊ��s��̐ݒ�
	static void set_world_matrices(const Matrix world_matrices[]);
	// �{�[�����̎擾
	static int bone_count();
	// ���[�V�����̏I�����Ԃ̎擾
	static float end_time(int motion);

private:
	// �o�C���h���̃��f��
	static int			model_;
	// �{�[���̍ő吔
	static const int	BoneMax{ 256 };
	// �{�[���̃��[�J���ϊ��s��
	static Matrix		local_matrices_[BoneMax];
	// �{�[���̃��[���h�ϊ��s��
	static Matrix		world_matrices_[BoneMax];
	// ���f���A�Z�b�g
	static ModelAsset	asset_;
};

#endif // !SKELETAL_MESH_H_