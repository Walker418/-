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