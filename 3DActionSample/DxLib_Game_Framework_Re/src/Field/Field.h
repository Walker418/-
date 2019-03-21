#ifndef FIELD_H_
#define FIELD_H_

#include "../Math/Vector3.h"

// クラス：フィールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Field
{
public:
	// コンストラクタ
	Field(int stage, int skybox);
	// 描画
	void draw() const;

	// 線分との衝突判定
	bool collide_line(const Vector3& start, const Vector3& end, Vector3* point = nullptr, Vector3* normal = nullptr);
	// 球体との衝突判定
	bool collide_sphere(const Vector3& center, float radius, Vector3* result = nullptr);
	// カプセルとの衝突判定
	bool collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result = nullptr);

	// フィールドの最大座標の取得
	Vector3 max_position() const;
	// フィールドの最小座標の取得
	Vector3 min_position() const;

	// フィールド内判定
	bool is_inside(const Vector3& position) const;
	// フィールド外判定
	bool is_outside(const Vector3& position) const;

	// コピー禁止
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

private:
	// ステージモデル
	int stage_;
	// スカイボックスモデル
	int skybox_;
};

#endif // !FIELD_H_