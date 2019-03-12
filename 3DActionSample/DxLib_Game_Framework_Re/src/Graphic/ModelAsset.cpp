#include "ModelAsset.h"
#include <DxLib.h>

// クラス：モデルアセット
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// デストラクタ
ModelAsset::~ModelAsset()
{
	// データを消去
	clear();
}

// モデルの読み込み
bool ModelAsset::load(int id, const std::string& file_name)
{
	// 同じIDのデータを削除
	erase(id);
	// モデルデータを読み込む
	asset_[id] = MV1LoadModel(file_name.c_str());

	// 読み込み結果を返す
	return asset_[id] != -1;
}

// モデルの削除
void ModelAsset::erase(int id)
{
	// 該当のモデルデータが存在する場合、データを削除
	if (asset_.count(id) > 0)
	{
		MV1DeleteModel(asset_[id]);
		asset_.erase(id);
	}
}

// データの消去
void ModelAsset::clear()
{
	// 全てのモデルデータを消去
	for (const auto model : asset_)
	{
		MV1DeleteModel(model.second);
	}
	asset_.clear();
}

// ハンドルの取得
int ModelAsset::operator[](int id) const
{
	return asset_.at(id);
}