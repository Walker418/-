#ifndef COLLISION_TRIANGLE_H_
#define COLLISION_TRIANGLE_H_

#include <DxLib.h>

// �N���X�F�Փ˔���p�O�p�`
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class CollisionTriangle
{
public:
	// �R���X�g���N�^
	CollisionTriangle(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3);
	// �O�p�`�̓����ɍ��W�͑��݂��邩
	bool is_inside(const VECTOR& point) const;
	// �O�p�`�̕ӂƋ��̂̏Փ˔���
	bool collide_edge_and_sphere(const VECTOR& center, float radius, VECTOR* result = nullptr);

private:
	// �O�p�`�̒��_
	VECTOR points_[3];
};

#endif // !COLLISION_TRIANGLE_H_