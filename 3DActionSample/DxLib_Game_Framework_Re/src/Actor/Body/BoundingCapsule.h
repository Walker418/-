#ifndef BOUNDING_CAPSULE_H_
#define BOUNDING_CAPSULE_H_

#include "Body.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix.h"

// クラス：衝突判定用カプセル
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class BoundingCapsule : public Body
{
public:
	// コンストラクタ
	BoundingCapsule(const Vector3& position, const Matrix& matrix, const float radius, const float length);
	// 平行移動
	virtual IBodyPtr translate(const Vector3& position) const override;
	// 座標変換
	virtual IBodyPtr transform(const Matrix& matrix) const override;
	// 衝突判定
	virtual bool is_collided(const IBody& other) const override;
	// カプセルとの衝突判定
	virtual bool intersects(const BoundingCapsule& other) const override;
	// 球体との衝突判定
	virtual bool intersects(const BoundingSphere& other) const override;
	// 描画
	virtual void draw() const override;

	// 平行移動
	BoundingCapsule translate_e(const Vector3& position) const;
	// 座標変換
	BoundingCapsule transform_e(const Matrix& matrix) const;
};

#endif // !BOUNDING_CAPSULE_H_