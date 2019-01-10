#ifndef COLOR_H_
#define COLOR_H_

// 構造体：カラー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
struct Color
{
	float r{ 1.0f };	// 赤成分
	float g{ 1.0f };	// 緑成分
	float b{ 1.0f };	// 青成分
	float a{ 1.0f };	// アルファ成分

	// デフォルトコンストラクタ
	Color() = default;
	// コンストラクタ
	Color(float r, float g, float b, float a);

	// 線形補間
	static Color Lerp(const Color& v1, const Color& v2, float amount);
	// 一致する各成分ペアの最も低い値を含むカラーを求める
	static Color Min(const Color& v1, const Color& v2);
	// 一致する各成分ペアの最も高い値を含むカラーを求める
	static Color Max(const Color& v1, const Color& v2);
	// 数値を制限
	static Color Clamp(const Color& value, const Color& min = Color(0.0f, 0.0f, 0.0f, 0.0f), const Color& max = Color(1.0f, 1.0f, 1.0f, 1.0f));

	// 定数
	static const Color Black;	// Color(0, 0, 0, 1)
	static const Color White;   // Color(1, 1, 1, 1)
};

// 演算子オーバーロード
Color& operator += (Color& c1, const Color& c2);
Color& operator -= (Color& c1, const Color& c2);
Color& operator *= (Color& c1, const Color& c2);
Color& operator *= (Color& c, float s);
Color& operator /= (Color& c, float s);

Color operator + (Color c1, const Color& c2);
Color operator - (Color c1, const Color& c2);
Color operator * (Color c1, const Color& c2);
Color operator * (Color c, float s);
Color operator * (float s, Color c);
Color operator / (Color c, float s);

#endif // !COLOR_H_