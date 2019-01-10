#include "StaticMesh.h"
#include <DxLib.h>

// クラス：スタティックメッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// バインド中のモデル
int StaticMesh::model_{ -1 };
// ボーンのワールド変換行列
Matrix StaticMesh::world_matrix_{ Matrix::Identity };
// モデルアセット
ModelAsset StaticMesh::asset_;

// 初期化
void StaticMesh::initialize()
{
	// 終了処理と同じ
	finalize();
}

// 終了処理
void StaticMesh::finalize()
{
	asset_.clear();
	model_ = -1;
	world_matrix_ = Matrix::Identity;
}

// 読み込み
bool StaticMesh::load(int id, const std::string& file_name)
{
	return asset_.load(id, file_name);
}

// 削除
void StaticMesh::erase(int id)
{
	model_ = (model_ == asset_[id]) ? -1 : model_;
	asset_.erase(id);
}

// メッシュのバインド
void StaticMesh::bind(int id)
{
	model_ = asset_[id];
}

// 描画
void StaticMesh::draw()
{
	MV1SetMatrix(model_, world_matrix_);
	MV1DrawModel(model_);
}

// ワールド変換行列の計算
void StaticMesh::transform(const Matrix & world)
{
	world_matrix_ = world;
}