#ifndef BODY_H_
#define BODY_H_

#include "IBody.h"
#include "Vector3.h"
#include "Matrix.h"

// クラス：衝突判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Body : public IBody
{
public:
	// コンストラクタ
	Body();
	// 球体のコンストラクタ
	Body(const ShapeType& type, const Vector3& position, const float radius);
	// カプセルのコンストラクタ
	Body(const ShapeType& type, const Vector3& position, const Matrix& matrix, const float radius, const float length);
	// デストラクタ
	~Body();

	// 接触判定のOn,Off
	virtual void set_active(bool active) override;
	// 接触判定は有効であるか
	virtual bool is_enable() const override;
	// 形状の取得
	virtual ShapeType type() const override;
	// 中心座標の取得
	virtual Vector3 position() const override;
	// 変換行列の取得
	virtual Matrix matrix() const override;
	// 半径の取得
	virtual float radius() const override;
	// 長さの取得
	virtual float length() const override;

	// 頂点の取得
	virtual Vector3 points(const int index) const override;

protected:
	// 衝突判定の形状
	ShapeType	type_;
	// 中心座標
	Vector3		position_;
	// 変換行列
	Matrix		matrix_;
	// 半径
	float		radius_;
	// 長さ
	float		length_;
	// 頂点
	Vector3		points_[4];
	// 接触判定は有効であるか
	bool		is_enable_;
};

#endif // !BODY_H_