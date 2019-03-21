#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Actor.h"
#include "../ActorPtr.h"

// �N���X�F�G�L��������
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class Enemy : public Actor
{
public:
	// �R���X�g���N�^
	Enemy(IWorld* world, const std::string& name, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::shared_ptr<DummyBody>());

	// �n�ʂƂ̐ڐG����
	void intersect_ground();
	// �ǂƂ̐ڐG����
	void intersect_wall();
	// ���W�̐���
	void clamp_position();

	// �v���C���[���擾
	ActorPtr get_player();
	// �v���C���[�̈ʒu���擾
	Vector3 get_player_position();
	// �ڕW�ւ̊p�x���擾�i�����t���j
	float get_angle_to_target(Vector3 target) const;
	// �ڕW�ւ̊p�x���擾�i���������j
	float get_unsigned_angle_to_target(Vector3 target) const;

	// �v���C���[�͑��݂��邩
	bool player_exists();
	// �v���C���[�͑O�ɂ��邩
	bool player_in_forward();
	// �v���C���[�͍U���������ɂ��邩
	bool player_in_range_distance(float distance);
	// �v���C���[�͍U���ł���p�x�ɂ��邩
	bool player_in_range_angle(float angle);

	// �v���C���[���U���ł��邩
	virtual bool can_attack_player() = 0;

protected:
	// ���݂̗̑�
	int				current_hp_;
	// ���ݗݐϒl
	int				current_wince_{ 0 };
	// �s���ҋ@����
	float			interval_{ 0.0f };

};

#endif // !ENEMY_H_