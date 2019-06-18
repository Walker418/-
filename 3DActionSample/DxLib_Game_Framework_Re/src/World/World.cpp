#include "World.h"
#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../Actor/Actor.h"
#include "../Graphic/Shader/BloomCB.h"
#include "../Graphic/Shader/ShaderID.h"
#include "../Graphic/Graphics3D.h"
#include "../Graphic/Shader/ShaderManager.h"

// �N���X�F���[���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
World::World()
{
	// �������������s��
	initialize();
}

// ������
void World::initialize()
{
	// �S�Ẵf�[�^������
	clear();
	// �A�N�^�[�O���[�v���A�N�^�[�O���[�v�}�l�[�W���[�ɒǉ�
	actors_.add_group(ActorGroup::None);			// ��������
	actors_.add_group(ActorGroup::Player);			// �v���C���[
	actors_.add_group(ActorGroup::PlayerAttack);	// �v���C���[�̍U��
	actors_.add_group(ActorGroup::Enemy);			// �G
	actors_.add_group(ActorGroup::EnemyAttack);		// �G�̍U��
	actors_.add_group(ActorGroup::Effect);			// �G�t�F�N�g
	actors_.add_group(ActorGroup::UI);				// UI

	// �V�F�[�_�[�p�萔�o�b�t�@�𐶐�
	cb_bloom_ = CreateShaderConstantBuffer(sizeof(BloomCB));
}

// �X�V
void World::update(float delta_time)
{
	// �e�A�N�^�[�̏�Ԃ��X�V
	actors_.update(delta_time);
	// �ڐG������s��
	actors_.collide(ActorGroup::Player, ActorGroup::Enemy);
	actors_.collide(ActorGroup::Player, ActorGroup::EnemyAttack);
	actors_.collide(ActorGroup::PlayerAttack, ActorGroup::Enemy);
	actors_.collide(ActorGroup::Enemy);
	// ���S�����A�N�^�[���폜
	actors_.remove();

	// �J�����̏�Ԃ��X�V
	camera_->update(delta_time);
	// ���C�g�̏�Ԃ��X�V
	light_->update(delta_time);
}

// �`��
void World::draw() const
{
	// �u���[���V�F�[�_�[�p�̃p�����[�^��ݒ�
	BloomCB bloom_param;						// �u���[���V�F�[�_�[�p�萔�o�b�t�@��錾
	bloom_param.g_BrightPassThreshold = 0.5f;	// ���o����P�x�̍ŏ��l
	bloom_param.g_BloomIntensity = 0.75f;		// �u���[���e�N�X�`���̋P�x
	bloom_param.g_BloomSaturation = 0.75f;		// �u���[���e�N�X�`���̍ʓx
	bloom_param.g_BaseIntensity = 0.8f;			// ���e�N�X�`���̋P�x
	bloom_param.g_BaseSaturation = 0.8f;		// ���e�N�X�`���̍ʓx

	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�̃A�h���X���擾
	auto cb = (BloomCB*)GetBufferShaderConstantBuffer(cb_bloom_);
	// �萔�o�b�t�@�̒l��ݒ�
	*cb = bloom_param;
	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���X�V���āA���e�𔽉f������
	UpdateShaderConstantBuffer(cb_bloom_);
	// �s�N�Z���V�F�[�_�[�̒萔�o�b�t�@��4�Ԗڂ̃X���b�g�Ɏw��
	SetShaderConstantBuffer(cb_bloom_, DX_SHADERTYPE_PIXEL, 4);

	// --------------------------------------------------
	// ���摜�Ƀ����_�����O
	// --------------------------------------------------
	rt_source_.begin();		// ���摜�̃����_�[�^�[�Q�b�g���N��
	Graphics3D::clear();	// ��ʂ��N���A

	// �����ŃV�F�[�_�[��K�p����A�N�^�[��`��
	camera_->draw();		// �J������ݒ�
	light_->draw();
	field_->draw();
	actors_.draw(ActorGroup::Player);
	actors_.draw(ActorGroup::Enemy);
	actors_.draw(ActorGroup::PlayerAttack);
	actors_.draw(ActorGroup::EnemyAttack);

	rt_source_.end();		// ���摜�̃����_�[�^�[�Q�b�g���I��
	// --------------------------------------------------

	// --------------------------------------------------
	// �P�x�𒊏o
	// --------------------------------------------------
	rt_bright_.begin();										// �P�x�̃����_�[�^�[�Q�b�g���N��
	ShaderManager::set_ps((int)ShaderID::PS_BrightPass);	// �P�x���o�V�F�[�_�[���w��
	SetUseTextureToShader(0, rt_source_.texture());			// ���摜�ɃV�F�[�_�[��������
	rt_bright_.draw();										// �P�x�̃����_�[�^�[�Q�b�g��`��
	rt_bright_.end();										// �P�x�̃����_�[�^�[�Q�b�g���I��

	// --------------------------------------------------

	// --------------------------------------------------
	// ���������u���[��������
	// --------------------------------------------------

	rt_bluer_h_.begin();									// ���������u���[�̃����_�[�^�[�Q�b�g���N��
	ShaderManager::set_ps((int)ShaderID::PS_GaussianBlurH);	// ���������u���[�V�F�[�_�[���w��
	SetUseTextureToShader(0, rt_bright_.texture());			// �P�x���o�����摜�ɃV�F�[�_�[��������
	rt_bluer_h_.draw();										// ���������u���[�̃����_�[�^�[�Q�b�g��`��
	rt_bluer_h_.end();										// ���������u���[�̃����_�[�^�[�Q�b�g���I��

	// --------------------------------------------------

	// --------------------------------------------------
	// ���������u���[��������
	// --------------------------------------------------

	rt_bluer_v_.begin();									// ���������u���[�̃����_�[�^�[�Q�b�g���N��
	ShaderManager::set_ps((int)ShaderID::PS_GaussianBlurV);	// ���������u���[�V�F�[�_�[���w��
	SetUseTextureToShader(0, rt_bluer_h_.texture());		// ���������u���[�ƍ��������摜�ɃV�F�[�_�[��������
	rt_bluer_v_.draw();										// ���������u���[�̃����_�[�^�[�Q�b�g��`��
	rt_bluer_v_.end();										// ���������u���[�̃����_�[�^�[�Q�b�g���I��

	// --------------------------------------------------

	// --------------------------------------------------
	// ���摜�ƃu���[�摜������
	// --------------------------------------------------

	rt_bloom_combine_.begin();								// �u���[�������p�̃����_�[�^�[�Q�b�g���N��
	ShaderManager::set_ps((int)ShaderID::PS_BloomCombine);	// �u���[�������V�F�[�_�[���w��
	// ���摜�ƃu���[�摜������
	SetUseTextureToShader(0, rt_source_.texture());
	SetUseTextureToShader(1, rt_bluer_v_.texture());
	rt_bloom_combine_.draw();								// �u���[�������p�̃����_�[�^�[�Q�b�g��`��
	rt_bloom_combine_.end();								// �u���[�������p�̃����_�[�^�[�Q�b�g���I��

	// --------------------------------------------------

	// --------------------------------------------------
	// �ēx���摜�Ƀ����_�����O���āA�V�F�[�_�[��K�p���Ȃ���ʂƓK�p�����ʂ�����
	// --------------------------------------------------

	rt_source_.begin();									// ���摜�̃����_�[�^�[�Q�b�g���N��
	DrawGraph(0, 0, rt_bloom_combine_.texture(), TRUE);	// �V�F�[�_�[���������f����`��
	// �����ŃV�F�[�_�[��K�p���Ȃ��A�N�^�[��`��
	camera_->draw();				// �J������ݒ�
	actors_.draw(ActorGroup::UI);	// �A�N�^�[��`��
	rt_source_.end();									// ���摜�̃����_�[�^�[�Q�b�g���I��

	// --------------------------------------------------

	// �ŏI�摜��`��
	DrawGraph(0, 0, rt_source_.texture(), FALSE);
}

// ���b�Z�[�W����
void World::handle_message(EventMessage message, void* param)
{
	// ���[���h�̃��b�Z�[�W����
	listener_(message, param);

	// �A�N�^�[�̃��b�Z�[�W����
	actors_.handle_message(message, param);
	camera_->handle_message(message, param);
	light_->handle_message(message, param);
}

// ���b�Z�[�W���X�i�[�̓o�^
void World::add_event_message_listener(EventMessageListener listener)
{
	listener_ = listener;
}

// �t�B�[���h�̒ǉ�
void World::add_field(const FieldPtr& field)
{
	field_ = field;
}

// �J�����̒ǉ�
void World::add_camera(const ActorPtr& camera)
{
	camera_ = camera;
}

// ���C�g�̒ǉ�
void World::add_light(const ActorPtr& light)
{
	light_ = light;
}

// �S�f�[�^�̏���
void World::clear()
{
	actors_.clear();
	field_ = nullptr;
	light_ = nullptr;
	camera_ = nullptr;
	listener_ = [](EventMessage, void*) {};
}

// �A�N�^�[��ǉ�
void World::add_actor(ActorGroup group, const ActorPtr& actor)
{
	actors_.add_actor(group, actor);
}

// �A�N�^�[�̎擾
ActorPtr World::find_actor(ActorGroup group, const std::string& name) const
{
	return actors_.find(group, name);
}

// �A�N�^�[���̎擾
unsigned int World::count_actor(ActorGroup group) const
{
	return actors_.count(group);
}

// �A�N�^�[���X�g�̏���
void World::each_actor(ActorGroup group, std::function<void(const ActorPtr&)> fn) const
{
	actors_.each(group, fn);
}

// ���b�Z�[�W�̑��M
void World::send_message(EventMessage message, void* param)
{
	handle_message(message, param);
}

// �t�B�[���h�̎擾
Field& World::field()
{
	return *field_;
}

// �J�����̎擾
ActorPtr World::camera()
{
	return camera_;
}

// ���C�g�̎擾
ActorPtr World::light()
{
	return light_;
}