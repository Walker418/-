#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include "Body.h"
#include "Vector3.h"

// クラス：衝突判定用球体
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class BoundingSphere : public Body
{
public:
	// コンストラクタ
	BoundingSphere(const Vector3& position, const float radius);
	// 平行移動
	virtual IBodyPtr translate(const Vector3& position) const override;
	// 座標変換
	virtual IBodyPtr transform(const Matrix& matrix) const override;
	// 衝突判定
	virtual bool is_collided(const IBody& other) const override;
	// 球体との衝突判定
	virtual bool intersects(const BoundingSphere& other) const override;
	// カプセルとの衝突判定
	virtual bool intersects(const BoundingCapsule& other) const override;
	// 描画
	virtual void draw() const override;

	// 平行移動
	BoundingSphere translate_e(const Vector3& position) const;
	// 座標変換
	BoundingSphere transform_e(const Matrix& matrix) const;
};

#endif // !BOUNDING_SPHERE_H_