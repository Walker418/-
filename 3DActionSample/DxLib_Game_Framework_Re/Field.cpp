#include "Field.h"
#include "CollisionMesh.h"
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

