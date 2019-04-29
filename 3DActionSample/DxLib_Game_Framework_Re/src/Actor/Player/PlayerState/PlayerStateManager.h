#ifndef PLAYER_STATE_H_
#define PLAYER_STATE_H_

#include "../../Actor.h"
#include "../../../World/IWorld.h"
#include "PlayerState.h"
#include <memory>

// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

using ActorWeak = std::weak_ptr<Actor>;		// �|�C���^�Ńv���C���[�̎Q�Ƃ��擾

// �N���X�F�v���C���[��ԊǗ�
class PlayerStateManager
{
public:
	// �R���X�g���N�^
	PlayerStateManager(ActorWeak player);
	PlayerStateManager(IWorld* world, ActorWeak player);
	// ���z�f�X�g���N�^
	virtual ~PlayerStateManager() { }
	// �J�n
	virtual void start();
	// �X�V
	virtual void update(float deltaTime);
	// �I��
	virtual void end();

protected:
	IWorld*		world_{ nullptr };	// ���[���h�̎Q��
	ActorWeak	player_;			// �v���C���[�̎Q��6
};

#endif // !PLAYER_STATE_H_