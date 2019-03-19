#include "SceneLoading.h"
#include "Scene.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math//Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"
#include "../Graphic/Graphics2D.h"
#include "../ID/SourceID.h"

// �N���X�F���[�f�B���O�V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �J�n
void SceneLoading::start()
{
	is_loaded_ = false;
	end_timer_ = 0.0f;
	is_end_ = false;
}

// �X�V
void SceneLoading::update(float delta_time)
{
	// �f�[�^���܂��ǂݍ���ł��Ȃ��ꍇ�A�f�[�^��ǂݍ���
	if (!is_loaded_)
	{
		// �f�[�^��ǂݍ���
		CollisionMesh::load(MESH_STAGE_CASTLE, "res/test_assets/castle/SampleStage_Castle.mv1");	// �X�e�[�W���f��
		Skybox::load(MESH_SKYBOX, "res/test_assets/skybox/skydome.mv1");							// �X�J�C�{�b�N�X���f��
		Billboard::load(0, "res/test_assets/Particle02.png");										// �r���{�[�h

		SkeletalMesh::load(MESH_PALADIN, "res/assets/Paladin/Paladin.mv1");							// �v���C���[���f��
		SkeletalMesh::load(MESH_GHOUL, "res/assets/Ghoul/ghoul.mv1");								// �G���G���f��
		SkeletalMesh::load(MESH_DRAGONBOAR, "res/assets/DragonBoar/DragonBoar.mv1");				// �{�X�G���f��

		Graphics2D::load(TEXTURE_HPGAUGE, "res/assets/textures/HPgauge.png");						// �̗̓Q�[�W
		Graphics2D::load(TEXTURE_HP, "res/assets/textures/HP.png");									// �̗͕\��

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
void SceneLoading::draw() const
{
	DrawString(0, 0, "�f�[�^�ǂݍ��ݒ��c", GetColor(255, 255, 255));
}

// �I�����Ă��邩
bool SceneLoading::is_end() const
{
	return is_end_;
}

// ���̃V�[����Ԃ�
Scene SceneLoading::next() const
{
	return Scene::GamePlay;
}

// �I��
void SceneLoading::end()
{
	// �������Ȃ�
}