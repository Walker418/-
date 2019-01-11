#include "Graphics2D.h"
#include <DxLib.h>

// クラス：2Dグラフィックス
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// スプライトデータ
std::unordered_map<int, AnimatedSprite> Graphics2D::sprite_map_;

// 初期化
void Graphics2D::initialize()
{
	sprite_map_.clear();
}

// 画像ファイルの読み込み
void Graphics2D::load(int id, const std::string& file_name)
{
	sprite_map_[id].load(file_name);
}

// 画像ファイルの読み込み（分割指定あり）
void Graphics2D::load(int id, const std::string& file_name, int row, int column)
{
	sprite_map_[id].load(file_name, row, column);
}

// 描画（画像全体）
void Graphics2D::draw(int id, const Vector2& position)
{
	sprite_map_[id].draw(position);
}

// 描画（画像の一部分を描画）
void Graphics2D::draw(int id, const Vector2& position, int x, int y, int w, int h)
{
	sprite_map_[id].draw(position, x, y, w, h);
}

// 描画（分割番号指定）
void Graphics2D::draw(int id, int div_no, const Vector2& position,
	const Vector2& center, const Vector2& scale, float rotation)
{
	sprite_map_[id].draw(div_no, position, center, scale, rotation);
}

// 描画（アニメーション指定）
void Graphics2D::draw(int id, int animation, float time, const Vector2& position,
	const Vector2& center, const Vector2& scale, float rotation)
{
	sprite_map_[id].draw(animation, time, position, center, scale, rotation);
}

// アニメーションの追加
void Graphics2D::add_animation(int id, int animation, int start, int end, float time)
{
	sprite_map_[id].add_animation(animation, start, end, time);
}

// キーフレームの追加
void Graphics2D::add_key_frame(int id, int animation, float time, int div_no)
{
	sprite_map_[id].add_key_frame(animation, time, div_no);
}

// キーフレームの追加
void Graphics2D::add_key_frame(int id, int animation, float time, int x, int y, int w, int h)
{
	sprite_map_[id].add_key_frame(animation, time, x, y, w, h);
}

// スプライトの削除
void Graphics2D::delete_sprite(int id)
{
	sprite_map_.erase(id);
}

// アニメーションの終了時間を取得
float Graphics2D::get_sprite_animation_end_time(int id, int animation)
{
	return sprite_map_[id].animation_end_time(animation);
}

// 背景色の設定
void Graphics2D::clear_color(float r, float g, float b)
{
	SetBackgroundColor(int(r * 255.0f), int(g * 255.0f), int(b * 255.0f));
}

// 終了
void Graphics2D::finalize()
{
	// 初期化と同じ処理
	initialize();
}