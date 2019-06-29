#include "SceneLoading.h"
#include "Scene.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math//Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"
#include "../Graphic/Graphics2D.h"
#include "../Graphic/Graphics3D.h"
#include "../Graphic/Shader/ShaderManager.h"
#include "../Graphic/Shader/ShaderID.h"
#include "../Sound/Sound.h"
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
		//------------------------------------------------------------
		// �����œǂݍ��ޑf�ނ����
		//------------------------------------------------------------

		// �X�e�[�W
		CollisionMesh::load(MESH_STAGE_CASTLE, "res/test_assets/castle/SampleStage_Castle.mv1");	// �X�e�[�W���f��
		Skybox::load(MESH_SKYBOX, "res/test_assets/skybox/skydome1.mv1");							// �X�J�C�{�b�N�X���f��
		Billboard::load(0, "res/test_assets/Particle02.png");										// �r���{�[�h
		// �L�����N�^�[
		SkeletalMesh::load(MESH_PALADIN, "res/assets/Paladin/Paladin.mv1");							// �v���C���[���f��
		SkeletalMesh::load(MESH_GHOUL, "res/assets/Ghoul/ghoul.mv1");								// �G���G���f��
		SkeletalMesh::load(MESH_DRAGONBOAR, "res/assets/DragonBoar/DragonBoar.mv1");				// �{�X�G���f��
		// 2D�摜
		Graphics2D::load(TEXTURE_TITLELOGO, "res/assets/texture/TitleLogo.png");					// �^�C�g�����S
		Graphics2D::load(TEXTURE_START, "res/assets/texture/Start.png");							// �X�^�[�g�\��
		Graphics2D::load(TEXTURE_HPGAUGE, "res/assets/texture/HPgauge.png");						// �̗̓Q�[�W
		Graphics2D::load(TEXTURE_HP, "res/assets/texture/HP.png");									// �̗͕\��
		Graphics2D::load(TEXTURE_P1MESSAGE, "res/assets/texture/P1message.png");					// �t�F�[�Y1�ڕW���b�Z�[�W
		Graphics2D::load(TEXTURE_P2MESSAGE, "res/assets/texture/P2message.png");					// �t�F�[�Y2�ڕW���b�Z�[�W
		// BGM
		Sound::load_bgm(BGM_TITLE, "res/assets/BGM/title.wav");										// �^�C�g�����BGM
		Sound::load_bgm(BGM_STAGE, "res/assets/BGM/stage.wav");										// �X�e�[�WBGM
		Sound::load_bgm(BGM_BOSS, "res/assets/BGM/boss.wav");										// �{�X��BGM
		// SE
		Sound::load_se(SE_START, "res/assets/SE/start.wav");										// �Q�[���X�^�[�gSE
		Sound::load_se(SE_SLASH, "res/assets/SE/slash.wav");										// �v���C���[�U��
		Sound::load_se(SE_BLOCK, "res/assets/SE/block.wav");										// �v���C���[�h��
		Sound::load_se(SE_PLAYER_ATK_HIT, "res/assets/SE/player_atk_hit.wav");						// �v���C���[�U������
		Sound::load_se(SE_ENEMY_ATK_LIGHT, "res/assets/SE/enemy_atk_light.wav");					// �G�̍U���i���j
		Sound::load_se(SE_ENEMY_ATK_HEAVY, "res/assets/SE/enemy_atk_heavy.wav");					// �G�̍U���i��j
		Sound::load_se(SE_BOSS_TACKLE, "res/assets/SE/boss_tackle.wav");							// �{�X�G�̃^�b�N��
		Sound::load_se(SE_BOSS_ROAR, "res/assets/SE/boss_roar.mp3");								// �{�X�G�̙��K
		// �V�F�[�_�[
		ShaderManager::load_ps((int)ShaderID::PS_BrightPass, "res/shader/BrightPass.cso");			// �P�x���o�V�F�[�_�[
		ShaderManager::load_ps((int)ShaderID::PS_GaussianBlurH, "res/shader/GaussianBlurH.cso");	// ���������u���[�V�F�[�_�[
		ShaderManager::load_ps((int)ShaderID::PS_GaussianBlurV, "res/shader/GaussianBlurV.cso");	// ���������u���[�V�F�[�_�[
		ShaderManager::load_ps((int)ShaderID::PS_BloomCombine, "res/shader/BloomCombine.cso");		// �u���[�������V�F�[�_�[

		//------------------------------------------------------------

		// �ǂݍ��ޔ����True�ɂ���i�ǂݍ��ݏI���j
		is_loaded_ = true;
	}
	// �f�[�^��ǂݍ��񂾏ꍇ�A�I���^�C�}�[���쓮���A2�b��Ƀv���C�V�[���ֈڍs
	else
	{
		if (end_timer_.is_time_out())
		{
			is_end_ = true;
		}

		end_timer_.update(delta_time);
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
	return Scene::Title;
}

// �I��
void SceneLoading::end()
{
	// �������Ȃ�
}