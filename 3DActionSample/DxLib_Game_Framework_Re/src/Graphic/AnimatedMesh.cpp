#include "AnimatedMesh.h"
#include "SkeletalMesh.h"

// �N���X�F�A�j���[�V�����t�����b�V��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
AnimatedMesh::AnimatedMesh(int mesh, int motion) :
	mesh_{ mesh }, animation_{ mesh, motion }
{ }

// �X�V
void AnimatedMesh::update(float delta_time)
{
	animation_.update(delta_time);
}

// �`��
void AnimatedMesh::draw() const
{
	SkeletalMesh::bind(mesh_);
	SkeletalMesh::set_world_matrices(world_matrices_.data());
	SkeletalMesh::draw();
}

// ���[�V�����̕ύX
void AnimatedMesh::change_motion(int motion)
{
	animation_.change_motion(motion);
}

// �X�P���g���̕ϊ��s��̌v�Z
void AnimatedMesh::transform(const Matrix& world)
{
	SkeletalMesh::bind(mesh_);
	SkeletalMesh::set_local_matrices(animation_.local_matrices().data());
	SkeletalMesh::transform(world);
	SkeletalMesh::get_world_matrices(world_matrices_.data());
}

// �ϊ��s��̎擾
const Matrix& AnimatedMesh::bone_matrix(int no) const
{
	return world_matrices_[no];
}

// ���[�V�����̏I�����Ԃ̎擾
float AnimatedMesh::motion_end_time() const
{
	return animation_.end_time();
}

// ���[�V�����̍Đ����x�̕ύX
void AnimatedMesh::change_speed(float speed)
{
	animation_.change_speed(speed);
}

// ���[�V�����̍Đ����x�̃��Z�b�g
void AnimatedMesh::reset_speed()
{
	animation_.reset_speed();
}