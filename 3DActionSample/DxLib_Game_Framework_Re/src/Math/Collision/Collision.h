#ifndef COLLISION_H_
#define COLLISION_H_

// クラス：衝突処理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

struct Vector3;
struct Matrix;

class Collision
{
public:
	// 球体同士の衝突処理
	static bool sphere_to_sphere(const Vector3& p1, const float r1, const Vector3& p2, const float r2);

	// カプセル同士の衝突処理
	static bool capsule_to_capsule(const Vector3 p1[], const float& r1, const Vector3 p2[], const float& r2);
	// カプセル同士の衝突処理
	static bool capsule_to_capsule(const Vector3& p1, const Matrix& m1, const float& l1, const float& r1, const Vector3& p2, const Matrix& m2, const float& l2, const float& r2);

	// 球体とカプセルの衝突処理
	static bool sphere_to_capsule(const Vector3& sphere, const float& sphere_r, const Vector3 capsule[], const float& capsule_r);
	// 球体とカプセルの衝突処理
	static bool sphere_to_capsule(const Vector3& sphere_p, const float& sphere_r, const Vector3& cap_pos, const Matrix& cap_mat, const float& cap_len, const float& cap_radius);
};

#endif // !COLLISION_H_