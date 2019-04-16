#include "SceneLoading.h"
#include "Scene.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math//Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"
#include "../Graphic/Graphics2D.h"
#include "../Graphic/Graphics3D.h"
#include "../Graphic/Shader/ShaderManager.h"
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
		// ������
		Graphics2D::initialize();
		Graphics3D::initialize();
		SkeletalMesh::initialize();
		CollisionMesh::initialize();
		Skybox::initialize();
		Billboard::initialize();
		ShaderManager::initialize();
		Sound::initialize();

		// �f�[�^��ǂݍ���
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
		// �V�F�[�_�[
		ShaderManager::load_vs(SHADER_VERTEX_SHADER, "VertexShader.cso");							// ���_�V�F�[�_�[
		ShaderManager::load_ps(SHADER_PIXEL_SHADER, "PixelShader.cso");								// �s�N�Z���V�F�[�_�[
		ShaderManager::load_vs(SHADER_SKYBOX_VERTEX_SHADER, "SkyboxVertexShader.cso");				// �X�J�C�{�b�N�X�p���_�V�F�[�_�[
		ShaderManager::load_ps(SHADER_SKYBOX_PIXEL_SHADER, "SkyboxPixelShader.cso");				// �X�J�C�{�b�N�X�p�s�N�Z���V�F�[�_�[
		// ���}�b�v
		ShaderManager::load_cubemap(CUBEMAP_TEST_SPECULAR_HDR, "res/assets/cubemap/TestSpecularHDR.dds");
		ShaderManager::load_cubemap(CUBEMAP_TEST_DIFFUSE_HDR, "res/assets/cubemap/TestDiffuseHDR.dds");
		ShaderManager::load_cubemap(CUBEMAP_TEST_BRDF, "res/assets/cubemap/TestBrdf.dds");
		ShaderManager::load_cubemap(CUBEMAP_TEST_ENV_HDR, "res/assets/cubemap/TestEnvHDR.dds");

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
	return Scene::Title;
}

// �I��
void SceneLoading::end()
{
	// �������Ȃ�
}