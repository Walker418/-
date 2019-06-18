#ifndef MATRIX_H_
#define MATRIX_H_

#include <DxLib.h>

// 構造体：変換行列
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

struct Vector3;		// 3Dベクトル構造体
struct Quaternion;	// クオータニオン構造体

struct Matrix
{
	// 単位行列
	float m[4][4]
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// デフォルトコンストラクタ
	Matrix() = default;
	// コンストラクタ
	Matrix
	(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	// 拡大縮小行列を作成
	static Matrix CreateScale(const Vector3& scale);
	// 平行移動行列を作成
	static Matrix CreateTranslation(const Vector3& translation);
	// 任意のベクトルの周囲を回転する行列を作成
	static Matrix CreateFromAxisAngle(const Vector3& axis, float degree);
	// クォータニオンから回転行列を作成
	static Matrix CreateFromQuaternion(const Quaternion& rotate);
	// 指定したヨー、ピッチ、ロールで行列を作成
	static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);
	// x軸まわりの回転行列を作成
	static Matrix CreateRotationX(float degree);
	// y軸まわりの回転行列を作成
	static Matrix CreateRotationY(float degree);
	// z軸まわりの回転行列を作成
	static Matrix CreateRotationZ(float degree);

	// ワールド変換行列を作成
	static Matrix CreateWorld(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
	// ワールド変換行列を作成
	static Matrix CreateWorld(const Vector3& scale, const Matrix& rotation, const Vector3& translation);
	// ワールド変換行列を作成
	static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);

	// 遠景行列を作成
	static Matrix CreatePerspectiveFieldOfView(float fov, float aspect, float near, float far);
	// 視野行列を作成
	static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);

	// スクリーン座標行列を作成
	static Matrix CreateScreen(float x, float y, float width, float height);

	// 逆行列を求める
	static Matrix Invert(const Matrix& matrix);
	// 逆行列を求める（回転と平行移動のみ）
	static Matrix InvertFast(const Matrix& matrix);
	// 行と列を入れ替える
	static Matrix Transpose(const Matrix& matrix);
	// 線形補間処理
	static Matrix Lerp(const Matrix& v1, const Matrix& v2, float t);
	// 行列を分解
	void Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const;
	// 正規化
	static Matrix NormalizeRotationMatrix(const Matrix& matrix);
	// 正規化
	Matrix& NormalizeRotationMatrix();

	// 前方ベクトルを取得
	Vector3 Forward() const;
	// 前方ベクトルを設定
	Matrix& Forward(const Vector3& forward);
	// 後方ベクトルを取得
	Vector3 Backward() const;
	// 後方ベクトルを設定
	Matrix& Backward(const Vector3& backward);
	// 上方ベクトルを取得
	Vector3 Up() const;
	// 上方ベクトルを設定
	Matrix& Up(const Vector3& up);
	// 下方ベクトルを取得
	Vector3 Down() const;
	// 下方ベクトルを設定
	Matrix& Down(const Vector3& down);
	// 左ベクトルを取得
	Vector3 Left() const;
	// 左ベクトルを設定
	Matrix& Left(const Vector3& left);
	// 右ベクトルを取得
	Vector3 Right() const;
	// 右ベクトルを設定
	Matrix& Right(const Vector3& rignt);
	// 平行移動ベクトルを取得
	Vector3 Translation() const;
	// 平行移動ベクトルを設定
	Matrix& Translation(const Vector3& translation);

	// スケール値を取得
	Vector3 Scale() const;
	// 回転の取得
	Quaternion Rotation() const;
	// 回転行列の取得
	Matrix RotationMatrix() const;

	// 単位行列の定数
	static const Matrix Identity;
	// ゼロ行列の定数
	static const Matrix Zero;

#ifdef  DX_LIB_H
	// Dxlib用変換関数
	Matrix(const DxLib::MATRIX& mat)
	{
		memcpy(this, &mat, sizeof(Matrix));
	}
	// DxLibの行列に変換
	operator DxLib::MATRIX() const
	{
		DxLib::MATRIX result;
		memcpy(&result, this, sizeof(DxLib::MATRIX));
		return result;
	}
#endif
};

// 演算子オーバーロード
Matrix& operator += (Matrix& m1, const Matrix& m2);
Matrix& operator -= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m, float s);
Matrix& operator /= (Matrix& m, float s);

Matrix operator * (Matrix m1, const Matrix& m2);
Matrix operator * (Matrix m, float s);
Matrix operator * (Matrix s, float m);
Vector3 operator * (const Vector3& v, const Matrix& m);

Matrix operator + (Matrix m1, const Matrix& m2);
Matrix operator - (Matrix m1, const Matrix& m2);
Matrix operator * (Matrix m, float s);
Matrix operator * (float s, Matrix m);
Matrix operator / (Matrix m, float s);
Matrix operator - (const Matrix& v);

#endif // !MATRIX_H_