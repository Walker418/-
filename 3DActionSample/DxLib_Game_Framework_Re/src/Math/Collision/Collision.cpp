#include "Collision.h"
#include <DxLib.h>
#include "../../Math/Vector3.h"
#include "../../Math/Matrix.h"
#include <algorithm>

// クラス：衝突処理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 球体同士の衝突処理
bool Collision::sphere_to_sphere(const Vector3& p1, const float r1, const Vector3& p2, const float r2)
{
	return Vector3::Distance(p1, p2) <= r1 + r2;
}

// カプセル同士の衝突処理
bool Collision::capsule_to_capsule(const Vector3 p1[], const float& r1, const Vector3 p2[], const float& r2)
{
	VECTOR p1_start = p1[0];
	VECTOR p1_end = p1[1];
	VECTOR p2_start = p2[0];
	VECTOR p2_end = p2[1];

	return HitCheck_Capsule_Capsule(p1_start, p1_end, r1, p2_start, p2_end, r2);
}

// カプセル同士の衝突処理
bool Collision::capsule_to_capsule(const Vector3& p1, const Matrix& m1, const float& l1, const float& r1, const Vector3& p2, const Matrix& m2, const float& l2, const float& r2)
{
	Vector3 point1[2], point2[2];
	point1[0] = p1 + Vector3::Up * l1 * m1;
	point1[1] = p1 + Vector3::Down * l1 * m1;
	point2[0] = p2 + Vector3::Up * l2 * m2;
	point2[1] = p2 + Vector3::Down * l2 * m2;

	return capsule_to_capsule(point1, r1, point2, r2);
}

// 球体とカプセルの衝突処理
bool Collision::sphere_to_capsule(const Vector3& sphere, const float& sphere_r, const Vector3 capsule[], const float& capsule_r)
{
	VECTOR sphere_pos = sphere;
	VECTOR capsule_start = capsule[0];
	VECTOR capsule_end = capsule[1];

	return Segment_Point_MinLength_Square(capsule_start, capsule_end, sphere_pos) < std::powf(sphere_r + capsule_r, 2);
}

// 球体とカプセルの衝突処理
bool Collision::sphere_to_capsule(const Vector3& sphere_p, const float& sphere_r, const Vector3& cap_pos, const Matrix& cap_mat, const float& cap_len, const float& cap_radius)
{
	Vector3 points[2];
	points[0] = cap_pos + Vector3::Up * cap_len * cap_mat;
	points[1] = cap_pos + Vector3::Down * cap_len * cap_mat;

	return sphere_to_capsule(sphere_p, sphere_r, points, cap_radius);
}
