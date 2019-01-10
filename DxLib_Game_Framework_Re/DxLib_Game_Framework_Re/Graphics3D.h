#ifndef GRAPHICS_3D_H_
#define GRAPHICS_3D_H_

#include "Vector3.h"
#include "Color.h"
#include "Matrix.h"

// 構造体：ライト（光源）
struct Light
{
	// 列挙型：ライトの種類
	enum class LightType
	{
		Directional,	// 平行光源
		Point,			// ポイントライト
		Spot,			// スポットライト
	};

	LightType	type{ LightType::Directional };		// ライトの種類
	Vector3		direction{ 1.0f, -1.0f, 1.0f };		// ライトの方向
	Vector3		position{ 0.0f, 0.0f, 0.0f };		// ライトの座標
	Color		ambient{ 0.3f, 0.3f, 0.3f, 1.0f };	// 環境光
	Color		diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };	// 拡散反射光
	Color		specular{ 1.0f, 1.0f, 1.0f ,1.0f };	// 鏡面反射光
	float		attenuation0{ 0.1f };				// 減衰パラメータ1
	float		attenuation1{ 0.1f };				// 減衰パラメータ2
	float		attenuation2{ 0.1f };				// 減衰パラメータ3
	float		range{ 2000.0f };					// ライトの有効距離
	float		out_angle{ 114.0f };				// スポットライトの外側角度
	float		in_angle{ 114.0f };					// スポットライトの内側角度
};

// 列挙型：ブレンドモード
enum class BlendMode
{
	None,		// 無効
	Alpha,		// アルファブレンド
	Add,		// 加算ブレンド
	Sub,		// 減算ブレンド
};

// クラス：3Dグラフィックス
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Graphics3D
{
public:
	// 初期化
	static void initialize();
	// 終了
	static void finalize();

	// スクリーンの消去
	static void clear();
	// スクリーン消去カラーの設定
	static void clear_color(float r, float g, float b);

	// ビューポートの設定
	static void set_viewport(int x, int y, int w, int h);
	// 視野変換行列の設定
	static void set_view_matrix(const Matrix& view);
	// 透視変換行列の設定
	static void set_projection_matrix(const Matrix& projection);
	// 視野変換行列の取得
	static Matrix get_view_matrix();
	// 透視変換行列の取得
	static Matrix get_projection_matrix();

	// ライトの設定
	static void set_light(const Light& light);
	// ライトの取得
	static const Light& get_light();
	// ライティングの有効化
	static void enable_lighting();
	// ライティングの無効化
	static void disable_lighting();
	// Zバッファの有効化
	static void enable_z_buffer();
	// Zバッファの無効化
	static void disable_z_buffer();
	// Zバッファへの書き込みの許可
	static void enable_write_z_buffer();
	// Zバッファへの書き込みの禁止
	static void disable_write_z_buffer();

	// ブレンドモードの設定
	static void blend_mode(BlendMode mode, float a = 1.0f);

private:
	// ライトのパラメータ
	static Light light_;
};

#endif // !GRAPHICS_3D_H_