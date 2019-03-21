#include "Field.h"
#include "../Math/Collision/CollisionMesh.h"
#include "Skybox.h"

// �N���X�F�t�B�[���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Field::Field(int stage, int skybox) :
	stage_{ stage }, skybox_{ skybox }
{
	//���f�����ݒ肳��Ă��Ȃ�������Ԃ�
	if (stage_ < 0 || skybox_ < 0)	return;
}

// �`��
void Field::draw() const
{
	// �X�J�C�{�b�N�X��`��
	Skybox::bind(skybox_);
	Skybox::draw();

	// �X�e�[�W��`��
	CollisionMesh::bind(stage_);
	CollisionMesh::draw();
}

// �����Ƃ̏Փ˔���
bool Field::collide_line(const Vector3& start, const Vector3& end, Vector3* point, Vector3* normal)
{
	CollisionMesh::bind(stage_);

	return CollisionMesh::collide_line(start, end, point, normal);
}

// ���̂Ƃ̏Փ˔���
bool Field::collide_sphere(const Vector3& center, float radius, Vector3* result)
{
	CollisionMesh::bind(stage_);

	return CollisionMesh::collide_sphere(center, radius, result);
}

// �J�v�Z���Ƃ̏Փ˔���
bool Field::collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result)
{
	CollisionMesh::bind(stage_);

	return CollisionMesh::collide_capsule(start, end, radius, result);
}

// �t�B�[���h�̍ő���W�̎擾
Vector3 Field::max_position() const
{
	return CollisionMesh::max_position();
}

// �t�B�[���h�̍ŏ����W�̎擾
Vector3 Field::min_position() const
{
	return CollisionMesh::min_position();
}

// �t�B�[���h������
bool Field::is_inside(const Vector3& position) const
{
	CollisionMesh::bind(stage_);

	if (position.x < min_position().x || position.x > max_position().x) return false;
	if (position.z < min_position().z || position.z > max_position().z) return false;

	return true;
}

// �t�B�[���h�O����
bool Field::is_outside(const Vector3& position) const
{
	return !is_inside(position);
}

