#include "Skybox.h"
#include <DxLib.h>

// クラス：スタティックメッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// バインド中のモデル
int Skybox::model_{ -1 };
// モデルアセット
ModelAsset Skybox::asset_;

// 初期化
void Skybox::initialize()
{
	// 終了処理と同じ
	finalize();
}

// 終了処理
void Skybox::finalize()
{
	// モデルアセットを消去
	asset_.clear();
	// モデルを消去
	model_ = -1;
}

// 読み込み
bool Skybox::load(int id, const std::string& file_name)
{
	return asset_.load(id, file_name);
}

// 削除
void Skybox::erase(int id)
{
	model_ = (model_ == asset_[id]) ? -1 : model_;
	asset_.erase(id);
}

// メッシュのバインド
void Skybox::bind(int id)
{
	model_ = asset_[id];
}

// 描画
void Skybox::draw()
{
	// 視野変換行列を取得
	MATRIX view_matrix = GetCameraViewMatrix();
	// 視野変換行列の平行移動量をクリア
	MATRIX skybox_matrix = view_matrix;
	skybox_matrix.m[3][0] = 0.0f;
	skybox_matrix.m[3][1] = 0.0f;
	skybox_matrix.m[3][2] = 0.0f;
	// 視野変換行列を設定
	SetCameraViewMatrix(skybox_matrix);
	// ライティングを無効化
	SetUseLighting(FALSE);
	// Zバッファに書き込みを行わない
	MV1SetWriteZBuffer(model_, FALSE);
	// モデルを表示
	MV1DrawModel(model_);
	// 視野変換行列を元に戻す
	SetCameraViewMatrix(view_matrix);
	// ライティングを有効化
	SetUseLighting(TRUE);
}