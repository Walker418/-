#ifndef VECTOR2_H_
#define VECTOR2_H_

// 構造体：2Dベクトル
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
struct Vector2
{
	float x{ 0.0f };	// x成分
	float y{ 0.0f };	// y成分

	// デフォルトコンストラクタ
	Vector2() = default;
	// コンストラクタ
	Vector2(float x, float y);

	// 内積を求める
	float Dot(const Vector2& other) const;
	// 外積を求める
	float Cross(const Vector2& other) const;
	// 長さを求める
	float Length() const;
	// 正規化したベクトルを求める
	Vector2 Normalize() const;

	// 角度からベクトルの作成
	static Vector2 FromAngle(float degree);
	// ベクトルが向いている角度を求める
	float ToAngle() const;
	// 回転処理
	Vector2 Rotate(float degree) const;
	// 2つのベクトルなす角度を求める
	float InnerAngle(const Vector2& other) const;

	// 距離を求める
	float Distance(const Vector2& other) const;
	// 線形補間処理
	Vector2 Lerp(const Vector2& end, float t) const;
	// 数値を制限
	Vector2 Clamp(const Vector2& min, const Vector2& max) const;
	// ターゲット方向のベクトルを求める
	Vector2 ToTarget(const Vector2& target) const;
	// ターゲット方向の角度を求める
	float ToTargetAngle(const Vector2& target) const;

	// ゼロベクトルなのか
	bool is_zero() const;

	// 定数
	static const Vector2 Zero;  //  0.0f  0.0f
	static const Vector2 One;   //  1.0f  1.0f
	static const Vector2 Left;  // -1.0f  0.0f
	static const Vector2 Right; //  1.0f  0.0f
	static const Vector2 Down;  //  0.0f  1.0f
	static const Vector2 Up;    //  0.0f -1.0f
};

// 演算子オーバーロード
Vector2& operator += (Vector2& lhs, const Vector2& rhs);
Vector2& operator -= (Vector2& lhs, const Vector2& rhs);
Vector2& operator *= (Vector2& lhs, float scalar);
Vector2& operator /= (Vector2& lhs, float scalar);

const Vector2 operator + (const Vector2& lhs, const Vector2& rhs);
const Vector2 operator - (const Vector2& lhs, const Vector2& rhs);
const Vector2 operator * (const Vector2& lhs, float scalar);
const Vector2 operator * (float scalar, const Vector2& rhs);
const Vector2 operator / (const Vector2& lhs, float scalar);
const Vector2 operator - (const Vector2& v);

bool operator == (const Vector2& lhs, const Vector2& rhs);
bool operator != (const Vector2& lhs, const Vector2& rhs);
bool operator < (const Vector2& lhs, const Vector2& rhs);

#endif // !VECTOR2_H_