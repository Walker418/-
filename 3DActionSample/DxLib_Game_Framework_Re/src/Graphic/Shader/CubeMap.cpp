#include "CubeMap.h"
#include <DxLib.h>

// クラス：キューブマップ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// デストラクタ
CubeMap::~CubeMap()
{
	clear();	// 全データを消去
}

// 読み込み
bool CubeMap::load(int id, const std::string& file_name)
{
	// 同じIDのデータを削除
	erase(id);
	// モデルデータを読み込む
	cube_map_[id] = LoadGraph(file_name.c_str());

	// 読み込み結果を返す
	return cube_map_[id] != -1;
}

// 環境マップの削除
void CubeMap::erase(int id)
{
	// 該当のシェーダーデータが存在する場合、データを削除
	if (cube_map_.count(id) > 0)
	{
		DeleteGraph(cube_map_[id]);
		cube_map_.erase(id);
	}
}

// データの消去
void CubeMap::clear()
{
	// 全てのシェーダーデータを消去
	for (const auto data : cube_map_)
	{
		DeleteGraph(data.second);
	}
	cube_map_.clear();
}

// ハンドルの取得
int CubeMap::operator[](int id) const
{
	return cube_map_.at(id);
}