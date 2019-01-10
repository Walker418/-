#ifndef COLLISION_TRIANGLE_H_
#define COLLISION_TRIANGLE_H_

#include <DxLib.h>

// クラス：衝突判定用三角形
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class CollisionTriangle
{
public:
	// コンストラクタ
	CollisionTriangle(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3);
	// 三角形の内側に座標は存在するか
	bool is_inside(const VECTOR& point) const;
	// 三角形の辺と球体の衝突判定
	bool collide_edge_and_sphere(const VECTOR& center, float radius, VECTOR* result = nullptr);

private:
	// 三角形の頂点
	VECTOR points_[3];
};

#endif // !COLLISION_TRIANGLE_H_