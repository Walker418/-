#include "Shader.h"
#include <DxLib.h>

// クラス：シェーダー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// デストラクタ
Shader::~Shader()
{
	clear();	// 全データを消去
}

// 頂点シェーダーの読み込み
bool Shader::load_vs(int id, const std::string & file_name)
{
	// 同じIDのデータを削除
	erase(id);
	// モデルデータを読み込む
	shader_[id] = LoadVertexShader(file_name.c_str());

	// 読み込み結果を返す
	return shader_[id] != -1;
}

// ピクセルシェーダーの読み込み
bool Shader::load_ps(int id, const std::string & file_name)
{
	// 同じIDのデータを削除
	erase(id);
	// モデルデータを読み込む
	shader_[id] = LoadPixelShader(file_name.c_str());

	// 読み込み結果を返す
	return shader_[id] != -1;
}

// シェーダーの削除
void Shader::erase(int id)
{
	// 該当のシェーダーデータが存在する場合、データを削除
	if (shader_.count(id) > 0)
	{
		DeleteShader(shader_[id]);
		shader_.erase(id);
	}
}

// データの消去
void Shader::clear()
{
	// 全てのシェーダーデータを消去
	for (const auto data : shader_)
	{
		DeleteShader(data.second);
	}
	shader_.clear();
}

// ハンドルの取得
int Shader::operator[](int id) const
{
	return shader_.at(id);
}

// 頂点シェーダーの指定
void Shader::set_vs(int id)
{
	SetUseVertexShader(shader_[id]);
}

// ピクセルシェーダーの指定
void Shader::set_ps(int id)
{
	SetUsePixelShader(shader_[id]);
}