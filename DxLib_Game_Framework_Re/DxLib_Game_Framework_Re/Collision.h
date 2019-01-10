#ifndef COLLISION_H_
#define COLLISION_H_

// �N���X�F�Փˏ���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

struct Vector3;
struct Matrix;

class Collision
{
public:
	// ���̓��m�̏Փˏ���
	static bool sphere_to_sphere(const Vector3& p1, const float r1, const Vector3& p2, const float r2);

	// �J�v�Z�����m�̏Փˏ���
	static bool capsule_to_capsule(const Vector3 p1[], const float& r1, const Vector3 p2[], const float& r2);
	// �J�v�Z�����m�̏Փˏ���
	static bool capsule_to_capsule(const Vector3& p1, const Matrix& m1, const float& l1, const float& r1, const Vector3& p2, const Matrix& m2, const float& l2, const float& r2);

	// ���̂ƃJ�v�Z���̏Փˏ���
	static bool sphere_to_capsule(const Vector3& sphere, const float& sphere_r, const Vector3 capsule[], const float& capsule_r);
	// ���̂ƃJ�v�Z���̏Փˏ���
	static bool sphere_to_capsule(const Vector3& sphere_p, const float& sphere_r, const Vector3& cap_pos, const Matrix& cap_mat, const float& cap_len, const float& cap_radius);
};

#endif // !COLLISION_H_