#ifndef LIGHT_H_
#define LIGHT_H_

#include "../Actor/Actor.h"

// �N���X�F���C�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Light : public Actor
{
public:
	// �R���X�g���N�^
	Light(IWorld* world, const Vector3& position);
	// �`��
	virtual void draw() const override;
};

#endif // !LIGHT_H_