#include "Billboard.h"

// クラス：ビルボード
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// バインド中のイメージ
int Billboard::image_{ -1 };
// テクスチャアセット
TextureAsset Billboard::asset_;

// 初期化
void Billboard::initialize()
{
	// 終了処理と同じ
	finalize();
}

// 終了処理
void Billboard::finalize()
{
	asset_.clear();
	image_ = -1;
}

// 読み込み
bool Billboard::load(int id, const std::string& file_name)
{
	return asset_.load(id, file_name);
}

// 削除
void Billboard::erase(int id)
{
	image_ = (image_ == asset_[id]) ? -1 : image_;
	asset_.erase(id);
}

// バインド
void Billboard::bind(int id)
{
	image_ = asset_[id];
}

// 描画
void Billboard::draw(const VECTOR& position, float size, float cx, float cy, float angle)
{
	DrawBillboard3D(position, cx, cy, size, angle, image_, TRUE);
}
