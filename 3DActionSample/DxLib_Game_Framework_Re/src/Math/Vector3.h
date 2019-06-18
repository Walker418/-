#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <DxLib.h>

// 構造体：3Dベクトル
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

struct Matrix;	// 変換行列構造体

struct Vector3
{
	float x{ 0.0f };	// x成分
	float y{ 0.0f };	// y成分
	float z{ 0.0f };	// z成分

	// デフォルトコンストラクタ
	Vector3() = default;
	// コンストラクタ
	Vector3(float x, float y, float z);

	// 長さを求める
	float Length() const;
	// 長さの平方を求める
	float LengthSquared() const;
	// 正規化したベクトルを求める
	Vector3& Normalize();
	// 指定したベクトルを正規化
	static Vector3 Normalize(const Vector3& v);
	// 2つのベクトルの内積を求める
	static float Dot(const Vector3& v1, const Vector3& v2);
	// 2つのベクトルの外積を求める
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	// 2つのベクトルの距離を求める
	static float Distance(const Vector3& v1, const Vector3& v2);
	// 2つのベクトルの距離の平方を求める
	static float DistanceSquared(const Vector3& v1, const Vector3& v2);
	// 線形補間処理
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	// 一致する各成分ペアの最も低い値を含むベクトルを求める
	static Vector3 Min(const Vector3& v1, const Vector3& v2);
	// 一致する各成分ペアの最も高い値を含むベクトルを求める
	static Vector3 Max(const Vector3& v1, const Vector3& v2);
	// 数値を制限
	static Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max);
	// 行列を3Dベクトルに変換
	static Vector3 Transform(const Vector3& position, const Matrix& matrix);
	// 行列を法線ベクトルに変換
	static Vector3 TransformNormal(const Vector3& position, const Matrix& matrix);
	// ヨー・ピッチからベクトルを作成
	static Vector3 CreateFromYawPitch(float yaw, float pitch);
	// ヨーを求める
	float Yaw() const;
	// ピッチを求める
	float Pitch() const;
	//  2つのベクトル間の鋭角を求める
	static float Angle(const Vector3& from, const Vector3& to);

	// 定数
	static const Vector3 Up;		// Vector3( 0,  1,  0)
	static const Vector3 Down;      // Vector3( 0, -1,  0)
	static const Vector3 Left;      // Vector3(-1,  0,  0)
	static const Vector3 Right;     // Vector3( 1,  0,  0)
	static const Vector3 Backward;  // Vector3( 0,  0, -1)
	static const Vector3 Forward;   // Vector3( 0,  0,  1)
	static const Vector3 UnitX;     // Vector3( 1,  0,  0)
	static const Vector3 UnitY;     // Vector3( 0,  1,  0)
	static const Vector3 UnitZ;     // Vector3( 0,  1,  1)
	static const Vector3 One;       // Vector3( 1,  1,  1)
	static const Vector3 Zero;      // Vector3( 0,  0,  0)

#ifdef DX_LIB_H
	// Dxlib用変換関数
	Vector3(const DxLib::VECTOR& v) : x(v.x), y(v.y), z(v.z) { }
	// DxLibのベクトルに変換
	operator DxLib::VECTOR() const
	{
		return { x, y, z };
	}
#endif
};

// 演算子オーバーロード
Vector3& operator += (Vector3& v1, const Vector3& v2);
Vector3& operator -= (Vector3& v1, const Vector3& v2);
Vector3& operator *= (Vector3& v, float s);
Vector3& operator /= (Vector3& v, float s);

Vector3 operator + (Vector3 v1, const Vector3& v2);
Vector3 operator - (Vector3 v1, const Vector3& v2);
Vector3 operator * (Vector3 v, float s);
Vector3 operator * (float s, Vector3 v);
Vector3 operator / (Vector3 v, float s);
Vector3 operator - (const Vector3& v);

#endif // !VECTOR3_H_