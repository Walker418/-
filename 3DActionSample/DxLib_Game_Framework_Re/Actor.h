#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include "Vector3.h"
#include "Matrix.h"
#include "IBodyPtr.h"
#include "DummyBody.h"

// �N���X�F�A�N�^�[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class IWorld;
enum class EventMessage;

class Actor
{
public:
	// �R���X�g���N�^
	Actor();
	// �R���X�g���N�^
	Actor(IWorld* world, const std::string& name, const Vector3& position, const IBodyPtr& body = std::shared_ptr<DummyBody>());
	// ���z�f�X�g���N�^
	virtual ~Actor();

public:
	// �X�V
	virtual void update(float delta_time);
	// �`��
	virtual void draw() const;
	// �Փ˃��A�N�V����
	virtual void react(Actor& other);
	// ���b�Z�[�W����
	virtual void handle_message(EventMessage message, void* param = nullptr);
	// �Փ˔���
	void collide(Actor& other);
	// ���S
	void die();
	// �Փ˂��Ă��邩
	bool is_collided(const Actor& other) const;
	// ���S���Ă��邩
	bool is_dead() const;

	// ���O�̎擾
	const std::string& name() const;
	// ���W�̎擾
	Vector3 position() const;
	// ��]�s��̎擾
	Matrix rotation() const;
	// �ϊ��s��̎擾
	Matrix pose() const;
	// �Փ˔���f�[�^�̎擾
	IBodyPtr body() const;

	// �R�s�[�֎~
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	// ���[���h
	IWorld*			world_{ nullptr };
	// ���O
	std::string		name_;
	// ���W
	Vector3			position_{ Vector3::Zero };
	// ��]�s��
	Matrix			rotation_{ Matrix::Identity };
	// �ړ���
	Vector3			velocity_{ Vector3::Zero };
	// �Փ˔���f�[�^
	IBodyPtr		body_;
	// ���S�t���O
	bool			is_dead_{ false };
};

#endif // !ACTOR_H_