#include "EnemyManager.h"

// �N���X�F�G�����A�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
EnemyManager::EnemyManager(IWorld* world) :
	Actor(world, "EnemyManager")
{
	generate_timer_ = 0.0f;
}

// �X�V
void EnemyManager::update(float delta_time)
{

}

// �`��
void EnemyManager::draw() const
{

}

// ���b�Z�[�W����
void EnemyManager::handle_message(EventMessage message, void* param)
{

}