#include "MyGame.h"
#include "Graphics3D.h"
#include "SkeletalMesh.h"
#include "CollisionMesh.h"
#include "Skybox.h"
#include "Billboard.h"
#include "Matrix.h"
#include "Vector3.h"

// �N���X�F3D�Q�[���t���[�����[�N�e�X�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
MyGame::MyGame() :
	Game{ 1024, 768, false }
{ }

// �J�n
void MyGame::start()
{
	// 3D���f����ǂݍ���
	SkeletalMesh::load(0, "test_assets/player.mv1");
	// �X�e�[�W���f����ǂݍ���
	CollisionMesh::load(0, "test_assets/castle/SampleStage_Castle.mv1");
	// �X�J�C�{�b�N�X���f����ǂݍ���
	Skybox::load(0, "test_assets/skybox/skydome.mv1");
	// �r���{�[�h��ǂݍ���
	Billboard::load(0, "test_assets/Particle02.png");
	
	// �t�B�[���h�𐶐�
	field_ = new Field(0, 0);
	// �L�����N�^�[�𐶐�
	mesh_ = new AnimatedMesh{ 0, 0 };
}

// �X�V
void MyGame::update(float delta_time)
{
	mesh_->update(delta_time);
	mesh_->transform(Matrix::Identity);
}

// �`��
void MyGame::draw()
{
	// ��ʂ��N���A
	Graphics3D::clear();

	// �J������ݒ�
	Graphics3D::set_view_matrix(Matrix::CreateLookAt({ -70.0f, 20.0f,-70.0f }, { 0.0f, 20.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));

	// �t�B�[���h��`��
	field_->draw();

	// �L�����N�^�[��`��
	mesh_->draw();

	// �r���{�[�h��`��
	Graphics3D::blend_mode(BlendMode::Add);
	Billboard::bind(0);
	Billboard::draw({ 0.0f, 30.0f, 0.0f }, 10.0f);
	Graphics3D::blend_mode(BlendMode::None);
}

// �I��
void MyGame::end()
{
	// �L�����N�^�[���폜
	delete mesh_;
	SkeletalMesh::erase(0);

	// �t�B�[���h���폜
	delete field_;
	CollisionMesh::erase(0);
	Skybox::erase(0);
	Billboard::erase(0);
}