#include "SceneLoadGamePlay.h"
#include "Scene.h"
#include "SkeletalMesh.h"
#include "CollisionMesh.h"
#include "Skybox.h"
#include "Billboard.h"
#include "SourceID.h"

// �N���X�F�Q�[���v���C�V�[�����\�[�X�̓ǂݍ���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �J�n
void SceneLoadGamePlay::start()
{
	is_loaded_ = false;
	end_timer_ = 0.0f;
	is_end_ = false;
}

// �X�V
void SceneLoadGamePlay::update(float delta_time)
{
	// �f�[�^���܂��ǂݍ���ł��Ȃ��ꍇ�A�f�[�^��ǂݍ���
	if (!is_loaded_)
	{
		// �f�[�^��ǂݍ���
		CollisionMesh::load(MESH_STAGE_CASTLE, "test_assets/castle/SampleStage_Castle.mv1");	// �X�e�[�W���f��
		Skybox::load(MESH_SKYBOX, "test_assets/skybox/skydome.mv1");							// �X�J�C�{�b�N�X���f��
		Billboard::load(0, "test_assets/Particle02.png");										// �r���{�[�h

		SkeletalMesh::load(MESH_PALADIN, "assets/Paladin/Paladin.mv1");							// �v���C���[���f��

		// �ǂݍ��ޔ����True�ɂ���i�ǂݍ��ݏI���j
		is_loaded_ = true;
	}
	// �f�[�^��ǂݍ��񂾏ꍇ�A�I���^�C�}�[���쓮���A2�b��Ƀv���C�V�[���ֈڍs
	else
	{
		// 60�t���[�� = 1�b
		if (end_timer_ >= 120.0f)
		{
			is_end_ = true;
		}

		end_timer_ += delta_time;
	}
}

// �`��
void SceneLoadGamePlay::draw() const
{
	DrawString(0, 0, "�f�[�^�ǂݍ��ݒ��c", GetColor(255, 255, 255));
}

// �I�����Ă��邩
bool SceneLoadGamePlay::is_end() const
{
	return is_end_;
}

// ���̃V�[����Ԃ�
Scene SceneLoadGamePlay::next() const
{
	return Scene::GamePlay;
}

// �I��
void SceneLoadGamePlay::end()
{
	// �������Ȃ�
}