#include "Field.h"
#include "../Math/Collision/CollisionMesh.h"
#include "Skybox.h"

// クラス：フィールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Field::Field(int stage, int skybox) :
	stage_{ stage }, skybox_{ skybox }
{
	//モデルが設定されていなかったら返す
	if (stage_ < 0 || skybox_ < 0)	return;
}

// 描画
void Field::draw() const
{
	// スカイボックスを描画
	Skybox::bind(skybox_);
	Skybox::draw();

	// ステージを描画
	CollisionMesh::bind(stage_);
	CollisionMesh::draw();
}

// 線分との衝突判定
bool Field::collide_line(const Vector3& start, const Vector3& end, Vector3* point, Vector3* normal)
{
	CollisionMesh::bind(stage_);

	return CollisionMesh::collide_line(start, end, point, normal);
}

// 球体との衝突判定
bool Field::collide_sphere(const Vector3& center, float radius, Vector3* result)
{
	CollisionMesh::bind(stage_);

	return CollisionMesh::collide_sphere(center, radius, result);
}

// カプセルとの衝突判定
bool Field::collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result)
{
	CollisionMesh::bind(stage_);

	return CollisionMesh::collide_capsule(start, end, radius, result);
}

// フィールドの最大座標の取得
Vector3 Field::max_position() const
{
	return CollisionMesh::max_position();
}

// フィールドの最小座標の取得
Vector3 Field::min_position() const
{
	return CollisionMesh::min_position();
}

// フィールド内判定
bool Field::is_inside(const Vector3& position) const
{
	CollisionMesh::bind(stage_);

	if (position.x < min_position().x || position.x > max_position().x) return false;
	if (position.z < min_position().z || position.z > max_position().z) return false;

	return true;
}

// フィールド外判定
bool Field::is_outside(const Vector3& position) const
{
	return !is_inside(position);
}

