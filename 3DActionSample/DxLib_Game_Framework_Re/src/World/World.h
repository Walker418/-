#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"
#include "../Actor/ActorGroupManager.h"
#include "../Actor/ActorPtr.h"
#include "../Field/FieldPtr.h"
#include "../Game/WindowSetting.h"
#include "../Graphic/Shader/RenderTarget.h"
#include <functional>

// �N���X�F���[���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

enum class EventMessage;

class World : public IWorld
{
public:
	// �C�x���g���b�Z�[�W���X�i�[
	using EventMessageListener = std::function<void(EventMessage, void*)>;

public:
	// �R���X�g���N�^
	World();
	// ������
	void initialize();
	// �X�V
	void update(float delta_time);
	// �`��
	void draw() const;
	// ���b�Z�[�W����
	void handle_message(EventMessage message, void* param);
	// ���b�Z�[�W���X�i�[�̓o�^
	void add_event_message_listener(EventMessageListener listener);
	// �t�B�[���h�̒ǉ�
	void add_field(const FieldPtr& field);
	// �J�����̒ǉ�
	void add_camera(const ActorPtr& camera);
	// ���C�g�̒ǉ�
	void add_light(const ActorPtr& light);
	// �S�f�[�^�̏���
	void clear();

	// �A�N�^�[��ǉ�
	virtual void add_actor(ActorGroup group, const ActorPtr& actor) override;
	// �A�N�^�[�̎擾
	virtual ActorPtr find_actor(ActorGroup group, const std::string& name) const override;
	// �A�N�^�[���̎擾
	virtual unsigned int count_actor(ActorGroup group) const override;
	// �A�N�^�[���X�g�̏���
	virtual void each_actor(ActorGroup group, std::function<void(const ActorPtr&)> fn) const override;
	// ���b�Z�[�W�̑��M
	virtual void send_message(EventMessage message, void* param = nullptr) override;
	// �t�B�[���h�̎擾
	virtual Field& field() override;
	// �J�����̎擾
	virtual ActorPtr camera() override;
	// ���C�g�̎擾
	virtual ActorPtr light() override;

	// �R�s�[�֎~
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	// �A�N�^�[�O���[�v�}�l�[�W���[
	ActorGroupManager		actors_;
	// �t�B�[���h
	FieldPtr				field_;
	// �J����
	ActorPtr				camera_;
	// ���C�g
	ActorPtr				light_;
	// ���b�Z�[�W���X�i�[
	EventMessageListener	listener_{ [](EventMessage, void*) {} };

	// �V�F�[�_�[�����p�����_�[�^�[�Q�b�g
	mutable RenderTarget rt_source_{ WindowSetting::WindowWidth , WindowSetting::WindowHeight };
	mutable RenderTarget rt_bright_{ WindowSetting::WindowWidth / 4 , WindowSetting::WindowHeight / 4 };
	mutable RenderTarget rt_bluer_h_{ WindowSetting::WindowWidth / 4 , WindowSetting::WindowHeight / 4 };
	mutable RenderTarget rt_bluer_v_{ WindowSetting::WindowWidth / 4 , WindowSetting::WindowHeight / 4 };
	mutable RenderTarget rt_bloom_combine_{ WindowSetting::WindowWidth , WindowSetting::WindowHeight };

	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@
	int cb_bloom_;	// �u���[���p
};

#endif // !WORLD_H_