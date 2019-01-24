#ifndef FIELD_H_
#define FIELD_H_

#include "Vector3.h"

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