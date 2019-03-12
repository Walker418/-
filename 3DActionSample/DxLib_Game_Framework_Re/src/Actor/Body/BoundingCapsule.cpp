#include "BoundingCapsule.h"
#include "BoundingSphere.h"
#include "../../Math/Collision/Collision.h"

// クラス：衝突判定用カプセル
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
BoundingCapsule::BoundingCapsule(const Vector3& position, const Matrix& matrix, const float radius, const float length) :
	Body(ShapeType::Capsule, position, matrix, radius, length)
{
	points_[0] = position + Vector3::Up * (length * 0.5f);
	points_[1] = position - Vector3::Up * (length * 0.5f);
}

// 平行移動
IBodyPtr BoundingCapsule::translate(const Vector3& position) const
{
	return std::make_shared<BoundingCapsule>(translate_e(position));
}

// 座標変換
IBodyPtr BoundingCapsule::transform(const Matrix& matrix) const
{
	return std::make_shared<BoundingCapsule>(transform_e(matrix));
}

// 衝突判定
bool BoundingCapsule::is_collided(const IBody& other) const
{
	// どちらかの判定が無効であれば、falseを返す
	if (!is_enable() || !other.is_enable()) return false;

	return other.intersects(*this);
}

// カプセルとの衝突判定
bool BoundingCapsule::intersects(const BoundingCapsule& other) const
{
	return Collision::capsule_to_capsule(position_, matrix_, length_, radius_, other.position_, other.matrix_, other.length_, other.radius_);
}

// 球体との衝突判定
bool BoundingCapsule::intersects(const BoundingSphere& other) const
{
	return Collision::sphere_to_capsule(other.position(), other.radius(), position_, matrix_, length_, radius_);
}

// 描画
void BoundingCapsule::draw() const
{
	// デバッグ中でしか描画しない
#ifdef _DEBUG

	Vector3 p[2];
	p[0] = position_ + Vector3::Up * length_ / 2 * matrix_;
	p[1] = position_ + Vector3::Down * length_ / 2 * matrix_;

	DrawCapsule3D(p[0], p[1], radius_, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
#endif
}

// 平行移動
BoundingCapsule BoundingCapsule::translate_e(const Vector3& position) const
{
	return BoundingCapsule(position_ + position, matrix_, length_, radius_);
}

// 座標変換
BoundingCapsule BoundingCapsule::transform_e(const Matrix& matrix) const
{
	return BoundingCapsule(position_ + matrix.Translation(), matrix_ * matrix.RotationMatrix(), length_ * matrix.Scale().y, radius_ * matrix.Scale().x);
}