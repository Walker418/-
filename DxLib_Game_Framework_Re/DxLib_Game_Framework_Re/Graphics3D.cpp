#include "Graphics3D.h"
#include "MathHelper.h"

// クラス：3Dグラフィックス
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// ライトのパラメータ
Light Graphics3D::light_;

// 初期化
void Graphics3D::initialize()
{
	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);
	// Zバッファを有効化
	enable_z_buffer();
	// Zバッファへの書き込みを許可
	enable_write_z_buffer();
	// デフォルトのライト設定
	set_light(Light());
	// ライティングを有効化
	enable_lighting();
}

// 終了
void Graphics3D::finalize()
{

}

// スクリーンの消去
void Graphics3D::clear()
{
	ClearDrawScreen();
}

// スクリーン消去カラーの設定
void Graphics3D::clear_color(float r, float g, float b)
{
	SetBackgroundColor(int(r * 255), int(g * 255), int(b * 255));
}

// ビューポートの設定
void Graphics3D::set_viewport(int x, int y, int w, int h)
{
	MATRIX viewport_matrix;
	CreateViewportMatrix(&viewport_matrix, x + (w / 2.0f), y + (h / 2.0f), float(w), float(h));
	SetTransformToViewport(&viewport_matrix);
	SetDrawArea(x, y, x + w, y + h);
}

// 視野変換行列の設定
void Graphics3D::set_view_matrix(const Matrix& view)
{
	SetCameraViewMatrix(view);
}

// 透視変換行列の設定
void Graphics3D::set_projection_matrix(const Matrix& projection)
{
	SetupCamera_ProjectionMatrix(projection);
}

// 視野変換行列の取得
Matrix Graphics3D::get_view_matrix()
{
	return GetCameraViewMatrix();
}

// 透視変換行列の取得
Matrix Graphics3D::get_projection_matrix()
{
	return GetCameraProjectionMatrix();
}

// ライトの設定
void Graphics3D::set_light(const Light& light)
{
	switch (light.type)
	{
	case Light::LightType::Directional:
		ChangeLightTypeDir(light.direction);
		break;
	case Light::LightType::Point:
		ChangeLightTypePoint(light.position, light.range, light.attenuation0, light.attenuation1, light.attenuation2);
		break;
	case Light::LightType::Spot:
		ChangeLightTypeSpot(light.position, light.direction,
			MathHelper::degree_to_radian(light.out_angle), MathHelper::degree_to_radian(light.in_angle),
			light.range, light.attenuation0, light.attenuation1, light.attenuation2);
		break;
	}

	SetLightDifColor(GetColorF(light.diffuse.r, light.diffuse.g, light.diffuse.b, light.diffuse.a));
	SetLightSpcColor(GetColorF(light.specular.r, light.specular.g, light.specular.b, light.specular.a));
	SetLightAmbColor(GetColorF(light.ambient.r, light.ambient.g, light.ambient.b, light.ambient.a));
	light_ = light;
}

// ライトの取得
const Light& Graphics3D::get_light()
{
	return light_;
}

// ライティングの有効化
void Graphics3D::enable_lighting()
{
	SetLightEnable(TRUE);
}

// ライティングの無効化
void Graphics3D::disable_lighting()
{
	SetLightEnable(FALSE);
}

// Zバッファの有効化
void Graphics3D::enable_z_buffer()
{
	SetUseZBuffer3D(TRUE);
}

// Zバッファの無効化
void Graphics3D::disable_z_buffer()
{
	SetUseZBuffer3D(FALSE);
}

// Zバッファへの書き込みの許可
void Graphics3D::enable_write_z_buffer()
{
	SetWriteZBuffer3D(TRUE);
}

// Zバッファへの書き込みの禁止
void Graphics3D::disable_write_z_buffer()
{
	SetWriteZBuffer3D(FALSE);
}

// ブレンドモードの設定
void Graphics3D::blend_mode(BlendMode mode, float a)
{
	switch (mode)
	{
	case BlendMode::None:
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	case BlendMode::Alpha:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(a * 255));
		break;
	case BlendMode::Add:
		SetDrawBlendMode(DX_BLENDMODE_ADD, int(a * 255));
		break;
	case BlendMode::Sub:
		SetDrawBlendMode(DX_BLENDMODE_SUB, int(a * 255));
		break;
	}
}