#ifndef QUATERNION_H_
#define QUATERNION_H_

struct Vector3;		// 3Dベクトル構造体
struct Matrix;		// 変換行列構造体

// 構造体：クオータニオン（四元数）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
struct Quaternion
{
	float x{ 0.0f };	// x成分
	float y{ 0.0f };    // y成分
	float z{ 0.0f };    // z成分
	float w{ 1.0f };    // w成分

	// デフォルトコンストラクタ
	Quaternion() = default;
	// コンストラクタ（各成分を直接入力）
	Quaternion(float x, float y, float z, float w);
	// コンストラクタ（指定軸回りの回転角度からクオータニオンを指定）
	Quaternion(const Vector3& axis, float angle);

	// 正規化
	static Quaternion Normalize(const Quaternion& q);
	// 正規化
	Quaternion& Normalize();

	// 内積を求める
	static float Dot(const Quaternion& q1, const Quaternion& q2);
	// 長さを求める
	float Length() const;

	// 線形補間
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
	// 回転行列からクオータニオンを作成
	static Quaternion CreateFromRotationMatrix(const Matrix& matrix);
	// 単位クオータニオン
	static const Quaternion Identity;
};

// 演算子オーバーロード
Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q, float s);
Quaternion& operator /= (Quaternion& q, float s);
Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);

Quaternion operator + (Quaternion q1, const Quaternion& q2);
Quaternion operator - (Quaternion q1, const Quaternion& q2);
Quaternion operator * (Quaternion q1, const Quaternion& q2);
Quaternion operator * (Quaternion q, float s);
Quaternion operator * (float s, Quaternion q);
Quaternion operator / (Quaternion q, float s);
Quaternion operator - (const Quaternion& q);

#endif // !QUATERNION_H_