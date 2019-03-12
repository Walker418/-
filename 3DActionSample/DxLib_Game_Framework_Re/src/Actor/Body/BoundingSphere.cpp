#include "BoundingSphere.h"
#include "BoundingCapsule.h"
#include "../../Math/Collision/Collision.h"

// クラス：衝突判定用球体
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
BoundingSphere::BoundingSphere(const Vector3& position, const float radius) :
	Body(ShapeType::Sphere, position, radius)
{ }

// 平行移動
IBodyPtr BoundingSphere::translate(const Vector3& position) const
{
	return std::make_shared<BoundingSphere>(translate_e(position));
}

// 座標変換
IBodyPtr BoundingSphere::transform(const Matrix& matrix) const
{
	return  std::make_shared<BoundingSphere>(transform_e(matrix));
}

// 衝突判定
bool BoundingSphere::is_collided(const IBody& other) const
{
	// どちらかの判定が無効であれば、falseを返す
	if (!is_enable() || !other.is_enable()) return false;

	return other.intersects(*this);
}

// 球体との衝突判定
bool BoundingSphere::intersects(const BoundingSphere& other) const
{
	return Collision::sphere_to_sphere(position_, radius_, other.position_, other.radius_);
}

// カプセルとの衝突判定
bool BoundingSphere::intersects(const BoundingCapsule& other) const
{
	return Collision::sphere_to_capsule(position_, radius_, other.position(), other.matrix(), other.length(), other.radius());
}

// 描画
void BoundingSphere::draw() const
{
	// デバッグ中でしか描画しない
#ifdef _DEBUG
	DrawSphere3D(position_, radius_, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
#endif
}

// 平行移動
BoundingSphere BoundingSphere::translate_e(const Vector3& position) const
{
	return BoundingSphere(position_ + position, radius_);
}

// 座標変換
BoundingSphere BoundingSphere::transform_e(const Matrix& matrix) const
{
	return BoundingSphere(position_ + matrix.Translation(), radius_ * matrix.Scale().y);
}