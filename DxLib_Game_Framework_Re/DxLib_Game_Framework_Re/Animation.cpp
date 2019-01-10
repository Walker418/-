#include "Animation.h"
#include "SkeletalMesh.h"

// �N���X�F�A�j���[�V��������
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Animation::Animation(int model, int motion) :
	model_{ model },
	motion_{ motion },
	prev_motion_{ motion }
{
	SkeletalMesh::bind(model_);									// ���b�V�����o�C���h
	SkeletalMesh::bind_animation(motion_, motion_timer_);		// �A�j���[�V�������o�C���h
	SkeletalMesh::get_local_matrices(local_matrices_.data());	// ���[�J���ϊ��s����擾
}

// �X�V
void Animation::update(float delta_time)
{
	// ���b�V�����o�C���h
	SkeletalMesh::bind(model_);
	// �A�j���[�V�������o�C���h
	SkeletalMesh::bind_animation(prev_motion_, prev_motion_timer_, motion_, motion_timer_, lerp_timer_ / LerpTime);
	// ���[�J���ϊ��s����擾
	SkeletalMesh::get_local_matrices(local_matrices_.data());

	// �A�j���[�V�����^�C�}�[���X�V
	motion_timer_ = std::fmod(motion_timer_ + 0.5f * delta_time, end_time());
	// ��ԃ^�C�}�[���X�V
	lerp_timer_ = std::fmin(lerp_timer_ + delta_time, LerpTime);
}

// ���[�V�����̕ύX
void Animation::change_motion(int motion)
{
	// ���݂Ɠ������[�V�����̏ꍇ�͉������Ȃ�
	if (motion == motion_) return;

	prev_motion_ = motion_;				// �O��̃��[�V�����ԍ����L�^
	prev_motion_timer_ = motion_timer_;	// �ŏI�A�j���[�V�����^�C�}�[���L�^
	motion_ = motion;					// ���[�V������ύX
	motion_timer_ = 0.0f;				// �A�j���[�V�����^�C�}�[�����Z�b�g
	lerp_timer_ = 0.0f;					// ��ԃ^�C�}�[�����Z�b�g
}

// ���ݍĐ����̃��[�V�����̎擾
int Animation::motion() const
{
	return motion_;
}

// �ϊ��s��̎擾
const Animation::Matrices& Animation::local_matrices() const
{
	return local_matrices_;
}

// �{�[�����̎擾
int Animation::bone_count() const
{
	SkeletalMesh::bind(model_);

	return SkeletalMesh::bone_count();
}

// �I�����Ԃ̎擾
float Animation::end_time() const
{
	SkeletalMesh::bind(model_);

	return SkeletalMesh::end_time(motion_);
}