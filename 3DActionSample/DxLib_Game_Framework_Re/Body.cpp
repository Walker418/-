#include "Body.h"

// クラス：ボディ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Body::Body() :
	type_{ ShapeType::None }, position_{ Vector3::Zero }, matrix_{ Matrix::Identity }, radius_{ 0.0f }, length_{ 0.0f }, is_enable_{ false }
{ }

// 球体のコンストラクタ
Body::Body(const ShapeType& type, const Vector3& position, const float radius) :
	type_{ ShapeType::Sphere }, position_{ position }, matrix_{ Matrix::Identity }, radius_{ radius }, length_{ 0.0f }, is_enable_{ true }
{ }

// カプセルのコンストラクタ
Body::Body(const ShapeType& type, const Vector3& position, const Matrix& matrix, const float radius, const float length) :
	type_{ ShapeType::Capsule }, position_{ position }, matrix_{ matrix }, radius_{ radius }, length_{ length }, is_enable_{ true }
{ }

// デストラクタ
Body::~Body()
{ }

// 接触判定のOn,Off
void Body::set_active(bool active)
{
	is_enable_ = active;
}

// 接触判定は有効であるか
bool Body::is_enable() const
{
	return is_enable_;
}

// 形状の取得
ShapeType Body::type() const
{
	return type_;
}

// 中心座標の取得
Vector3 Body::position() const
{
	return position_;
}

// 変換行列の取得
Matrix Body::matrix() const
{
	return matrix_;
}

// 半径の取得
float Body::radius() const
{
	return radius_;
}

// 長さの取得
float Body::length() const
{
	return length_;
}

// 頂点の取得
Vector3 Body::points(const int index) const
{
	return points_[index];
}