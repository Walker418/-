#include "TextureAsset.h"
#include <DxLib.h>

// クラス：テクスチャアセット
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// デストラクタ
TextureAsset::~TextureAsset()
{
	// データを消去
	clear();
}

// テクスチャの読み込み
bool TextureAsset::load(int id, const std::string& file_name)
{
	// 同じIDのデータを削除
	erase(id);
	// モデルデータを読み込む
	textures_[id] = LoadGraph(file_name.c_str());

	// 読み込み結果を返す
	return textures_[id] != -1;
}

// テクスチャの削除
void TextureAsset::erase(int id)
{
	// 該当のテクスチャデータが存在する場合、データを削除
	if (textures_.count(id) > 0)
	{
		DeleteGraph(textures_[id]);
		textures_.erase(id);
	}
}

// データの消去
void TextureAsset::clear()
{
	// 全てのテクスチャデータを消去
	for (const auto texture : textures_)
	{
		DeleteGraph(texture.second);
	}

	textures_.clear();
}

// ハンドルの取得
int TextureAsset::operator[](int id) const
{
	return textures_.at(id);
}
