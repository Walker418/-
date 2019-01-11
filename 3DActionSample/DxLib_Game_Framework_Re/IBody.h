#ifndef IBODY_H_
#define IBODY_H_

#include "IBodyPtr.h"
#include "ShapeType.h"

// インターフェース：衝突判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

struct Vector3;
struct Matrix;

class BoundingSphere;
class BoundingCapsule;

class IBody
{
public:
	// コンストラクタ
	IBody() { };
	// 仮想デストラクタ
	virtual ~IBody() = default;

	// 平行移動
	virtual IBodyPtr translate(const Vector3& position) const = 0;
	// 座標変換
	virtual IBodyPtr transform(const Matrix& matrix) const = 0;
	// 衝突判定
	virtual bool is_collide(const IBody& other) const = 0;
	// 球体との衝突判定
	virtual bool intersects(const BoundingSphere& other) const = 0;
	// カプセルとの衝突判定
	virtual bool intersects(const BoundingCapsule& other) const = 0;

	// 接触判定のOn,Off
	virtual void set_active(bool active) = 0;
	// 接触判定は有効であるか
	virtual bool is_enable() const = 0;
	// 描画
	virtual void draw() const = 0;
	// 形状の取得
	virtual ShapeType type() const = 0;
	// 中心座標の取得
	virtual Vector3 position() const = 0;
	// 変換行列の取得
	virtual Matrix matrix() const = 0;
	// 半径の取得
	virtual float radius() const = 0;
	// 長さの取得
	virtual float length() const = 0;

	// 頂点の取得
	virtual Vector3 points(const int index) const = 0;

};

#endif // !IBODY_H_