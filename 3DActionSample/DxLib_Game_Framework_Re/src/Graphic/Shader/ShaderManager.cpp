#include "ShaderManager.h"
#include <DxLib.h>

#include "PixelShaderCB.h"
#include "../../Actor/ActorManager.h"
#include "../../Actor/ActorGroup.h"

// クラス：シェーダー管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// シェーダーアセット
Shader ShaderManager::shader_asset_;
// キューブマップアセット
CubeMap ShaderManager::cubemap_asset_;
// ピクセルシェーダー用の変数バッファ
int ShaderManager::pscbhandle_{ -1 };

// 初期化
void ShaderManager::initialize()
{
	// 終了処理と同じ
	finalize();

	// ピクセルシェーダー用の変数バッファを作成
	pscbhandle_ = CreateShaderConstantBuffer(sizeof(PixelShaderCB));
}

// 終了処理
void ShaderManager::finalize()
{
	shader_asset_.clear();
	cubemap_asset_.clear();
	pscbhandle_ = -1;
}

// 頂点シェーダーの読み込み
bool ShaderManager::load_vs(int id, const std::string& file_name)
{
	return shader_asset_.load_vs(id, file_name);
}

// ピクセルシェーダーの読み込み
bool ShaderManager::load_ps(int id, const std::string& file_name)
{
	return shader_asset_.load_ps(id, file_name);
}

// キューブマップの読み込み
bool ShaderManager::load_cubemap(int id, const std::string& file_name)
{
	return cubemap_asset_.load(id, file_name);
}

// 頂点シェーダーの削除
void ShaderManager::erase_vs(int id)
{
	shader_asset_.erase(id);
}

// ピクセルシェーダーの削除
void ShaderManager::erase_ps(int id)
{
	shader_asset_.erase(id);
}

// キューブマップの削除
void ShaderManager::erase_cubemap(int id)
{
	cubemap_asset_.erase(id);
}

// 頂点シェーダーの指定
void ShaderManager::set_vs(int id)
{
	shader_asset_.set_vs(id);
}

// ピクセルシェーダーの指定
void ShaderManager::set_ps(int id)
{
	shader_asset_.set_ps(id);
}

// キューブマップの指定
void ShaderManager::set_cubemap(int id, int stage)
{
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP, stage);
	SetUseTextureToShader(stage, cubemap_asset_[id]);
}

// キューブマップの解除
void ShaderManager::off_cubemap(int stage)
{
	SetUseTextureToShader(11, -1);
}

// 更新
void ShaderManager::update(IWorld *world, float delta_time)
{
	// カメラの参照を取得しておく
	auto camera = world->camera();
	// ライトの参照を取得しておく
	auto light = world->light();

	// ピクセルシェーダー用の定数バッファのアドレスを取得
	auto cb = (PixelShaderCB*)GetBufferShaderConstantBuffer(pscbhandle_);
	// カメラの座標を設定
	cb->ViewPosition = camera->position();
	// ライトの座標を設定
	cb->LightPosition = light->position();
	// ライトのカラーを設定
	cb->LightColor = { 1.0f, 1.0f, 1.0f };
	// 定数バッファの内容を更新
	UpdateShaderConstantBuffer(pscbhandle_);
}

// シェーダーの有効化
void ShaderManager::shader_on()
{
	MV1SetUseOrigShader(TRUE);
}

// シェーダーの無効化
void ShaderManager::shader_off()
{
	MV1SetUseOrigShader(FALSE);
}