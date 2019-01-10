#ifndef DUMMY_BODY_H_
#define DUMMY_BODY_H_

#include "Body.h"

// クラス：ダミーの衝突判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class DummyBody : public Body
{
public:
	// コンストラクタ
	DummyBody() { }
	// 平行移動
	virtual IBodyPtr translate(const Vector3& position) const override { return std::make_shared<DummyBody>(); }
	// 座標変換
	virtual IBodyPtr transform(const Matrix& matrix) const override { return std::make_shared<DummyBody>(); }
	// 衝突判定
	virtual bool is_collide(const IBody& other) const override { return false; }
	// 球体との衝突判定
	virtual bool intersects(const BoundingSphere& other) const override { return false; }
	// カプセルとの衝突判定
	virtual bool intersects(const BoundingCapsule& other) const override { return false; }
	// 描画
	virtual void draw() const override { }
};

#endif // !DUMMY_BODY_H_