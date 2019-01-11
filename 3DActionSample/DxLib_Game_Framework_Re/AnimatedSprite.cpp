#include "AnimatedSprite.h"
#include "MathHelper.h"
#include <cmath>
#include <DxLib.h>

// クラス：アニメーション付きスプライト
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// デストラクタ
AnimatedSprite::~AnimatedSprite()
{
	clear();
}

// 画像ファイルの読み込み
void AnimatedSprite::load(const std::string& file_name)
{
	// 古いデータを削除
	clear();
	// 画像を読み込む
	graph_ = LoadGraph(file_name.c_str());
}

// 画像ファイルの読み込み（分割指定あり）
void AnimatedSprite::load(const std::string& file_name, int row, int column)
{
	// 画像データを読み込む
	load(file_name);
	// 画像サイズを取得
	int size_x = 0, size_y = 0;
	GetGraphSize(graph_, &size_x, &size_y);
	// 画像を分割
	int w = size_x / column;
	int h = size_y / row;
	for (int y = 0; y < size_y; y += h)
	{
		for (int x = 0; x < size_x; x += w)
		{
			div_graphs_.push_back(DerivationGraph(x, y, w, h, graph_));
		}
	}
}

// 描画（画像全体）
void AnimatedSprite::draw(const Vector2& position) const
{
	DrawGraphF(position.x, position.y, graph_, TRUE);
}

// 描画（画像の一部分を描画）
void AnimatedSprite::draw(const Vector2& position, int x, int y, int w, int h) const
{
	DrawRectGraphF(position.x, position.y, x, y, w, h, graph_, TRUE, FALSE);
}

// 描画（分割番号指定）
void AnimatedSprite::draw(int graph_no, const Vector2& position, const Vector2& center, const Vector2& scale, float rotation) const
{
	DrawRotaGraph3F(position.x, position.y, center.x, center.y, scale.x, scale.y,
		MathHelper::degree_to_radian(rotation), div_graphs_[graph_no], TRUE, FALSE);
}

// 描画（アニメーション指定）
void AnimatedSprite::draw(int animation, float time, const Vector2& position, const Vector2& center, const Vector2& scale, float rotation) const
{
	draw(find_key_frame(animation, time).second, position, center, scale, rotation);
}

// アニメーションの追加
void AnimatedSprite::add_animation(int animation, int start, int end, float time)
{
	// 古いデータを削除
	animations_[animation].clear();
	// アニメーションデータを追加
	for (int frame = 0; frame <= (end - start); ++frame)
	{
		add_key_frame(animation, (frame + 1) * time, start + frame);
	}
}

// キーフレームの追加
void AnimatedSprite::add_key_frame(int animation, float time, int graph_no)
{
	animations_[animation].emplace_back(time, graph_no);
}

// キーフレームの追加（分割指定あり）
void AnimatedSprite::add_key_frame(int animation, float time, int x, int y, int w, int h)
{
	div_graphs_.push_back(DerivationGraph(x, y, w, h, graph_));
	add_key_frame(animation, time, int(div_graphs_.size()) - 1);
}

// アニメーション終了時間の取得
float AnimatedSprite::animation_end_time(int animation) const
{
	return animations_.at(animation).back().first;
}

// 消去
void AnimatedSprite::clear()
{
	// 画像データを消去
	if (graph_ != -1)
	{
		DeleteSharingGraph(graph_);
		graph_ = -1;
		div_graphs_.clear();
	}
	// アニメーションデータを消去
	animations_.clear();
}

// キーフレームの検索
const AnimatedSprite::KeyFrame& AnimatedSprite::find_key_frame(int animation, float timer) const
{
	const auto& key_frames = animations_.at(animation);
	timer = fmod(timer, animation_end_time(animation) + 1.0f);
	for (const auto& key_frame : key_frames)
	{
		if (timer <= key_frame.first)
		{
			return key_frame;
		}
	}

	return key_frames.back();
}