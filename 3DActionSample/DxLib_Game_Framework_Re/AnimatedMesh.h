#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <array>
#include "Matrix.h"
#include "Animation.h"

// �N���X�F�A�j���[�V�����t�����b�V��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class AnimatedMesh
{
private:
	// �X�P���g���̕ϊ��s��
	using BoneMatrices = std::array<Matrix, 256>;

public:
	// �R���X�g���N�^
	AnimatedMesh(int mesh, int motion = 0);
	// �X�V
	void update(float delta_time);
	// �`��
	void draw() const;
	// ���[�V�����̕ύX
	void change_motion(int motion);
	// �X�P���g���̕ϊ��s��̌v�Z
	void transform(const Matrix& world);
	// �ϊ��s��̎擾
	const Matrix& bone_matrix(int no) const;
	// ���[�V�����̏I�����Ԃ̎擾
	float motion_end_time() const;

	// ���[�V�����̍Đ����x�̕ύX
	void change_speed(float speed);
	// ���[�V�����̍Đ����x�̃��Z�b�g
	void reset_speed();

private:
	// ���b�V��
	int				mesh_;
	// �A�j���[�V����
	Animation		animation_;
	// �X�P���g���̕ϊ��s��
	BoneMatrices	world_matrices_;
};

#endif // !ANIMATED_MESH_H_