#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"

// �N���X�F�v���C���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Player : public Actor
{
public:
	// �R���X�g���N�^
	Player(IWorld* world, const Vector3& position, const Matrix& rotation);


};

#endif // !PLAYER_H_